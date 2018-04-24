//Author: João C. Becker
#include<ptg.hpp>


bool showMesh;
bool showAxes;
bool rebuildflag;


int main(){
    printf("Compiled against GLFW %i.%i.%i\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
    
    //set the callback to show glfw errors
    glfwSetErrorCallback(errorCallback);
    
    printf("Calling glfwInit...\n");
    if (!glfwInit()){
        printf("Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Note: now cant work with Immediate mode, only with Core-profile
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    
    printf("Creating window...\n");
    GLFWwindow* window = glfwCreateWindow(ISCR_W, ISCR_H, "ptg", NULL, NULL);
    if (! window ){
        printf("Failed to create window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    //set GLFW callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//No cursor, camera mode only
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Failed to initialize GLAD");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    //Configure global opengl state to use z-buffer
    glEnable(GL_DEPTH_TEST);
    
    //read shader files
    Shader terrainProgram("shader/terrain.vs", "shader/terrain.fs");
    
    
    float myAxes[] = {
        1000.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        -1000.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.0f,  1000.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  -1000.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  0.0f, 1000.0f,  0.0f, 0.0f, 1.0f,
        0.0f,  0.0f, -1000.0f,  0.0f, 0.0f, 1.0f
    };
    unsigned int aVBO, aVAO;
    glGenVertexArrays(1, &aVAO);
    glGenBuffers(1, &aVBO);
    glBindVertexArray(aVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, aVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(myAxes), myAxes, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    
    
    rebuildflag = false;
    TerrainGen tManager(0, terrainProgram.ID);
    
    
    glViewport(0, 0, ISCR_W, ISCR_H);
    
    //creating variables
    int cscr_w=ISCR_W, cscr_h=ISCR_H;
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    double lastTime = glfwGetTime();
    unsigned frameCounter = 0, fps = 0.0;
    showMesh = false;
    showAxes = false;
    
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;
    
    Camera cam;
    glfwSetWindowUserPointer(window, &cam);
    
    unsigned myi=0;
    //mainLoop
    printf("Entering in the main loop...\n");
    while (!glfwWindowShouldClose(window)){
        float cFrameTime = glfwGetTime();
        deltaTime = cFrameTime - lastFrame;
        lastFrame = cFrameTime;
        frameCounter++;
        if(cFrameTime - lastTime >= 1.0){
            fps = frameCounter;
            frameCounter = 0;
            lastTime = cFrameTime;
        }
        
        printf("\r%u (%lf, %lf, %lf),(%lf, %lf, %lf)(%lf)  %u   ",myi++, cam.pos.x, cam.pos.y, cam.pos.z, cam.front.x, cam.front.y, cam.front.z, cam.zoomC, fps);
        fflush(stdout);
        
        //process holding keys
        processInputHoldingKey(window, deltaTime);
        
        //render
        //clear the collor buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear the buffers
        
        
        //active the shader and bind uniforms
        terrainProgram.use();
        
        // create transformations
        view  = cam.getViewMatrix();
        glfwGetWindowSize(window, &cscr_w, &cscr_h);//essa função é threadsafe
        projection = glm::perspective(glm::radians(cam.zoomC), (float)cscr_w / (float)cscr_h, 0.1f, 2200.0f);
        terrainProgram.setMat4("projection", projection);
        terrainProgram.setMat4("view", view);
        
        
        //render container
        if(showAxes){
            glBindVertexArray(aVAO);
            model = glm::mat4(1.0f);
            terrainProgram.setMat4("model", model);
            glDrawArrays(GL_LINES, 0, 6);
            
            
        }
        
        if(rebuildflag){
            
            tManager.rebuild(cam.pos);
            rebuildflag = false;
        }
        
        
        if(showMesh){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            
        }
        
        tManager.drawTerrain();
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    printf("\n");
    
    printf("Free memory space...\n");
    //de-allocate all resources once they've outlived their purpose
    
    glDeleteVertexArrays(1, &aVAO);
    glDeleteBuffers(1, &aVBO);
    tManager.shutDown();
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

static void errorCallback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){//input callBack
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)//close window
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_M && action == GLFW_PRESS)//intercalate, mesh and fill terrain
        showMesh = !showMesh;
    if (key == GLFW_KEY_K && action == GLFW_PRESS)//show/hide axes
        showAxes = !showAxes;
    if (key == GLFW_KEY_P && action == GLFW_PRESS){//move camera to specific location with specific angle view
        Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
        cam->pos = glm::vec3(42.237156, 142.904083, -199.660156);
        cam->front = glm::vec3(0.921904, -0.365689, -0.127925);
        
    }
    
    if (key == GLFW_KEY_R && action == GLFW_PRESS){//build a new terrain chunk in the camera position
        rebuildflag = true;
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height){//resize screen
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInputHoldingKey(GLFWwindow *window, float deltaTime){
    Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->moveForwarRelXZ(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->moveBackwardRelXZ(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->moveLeftRelXZ(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->moveRightRelXZ(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam->moveUpAbs(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cam->moveDownAbs(deltaTime);
}

static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos){
    Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
    cam->interpretMouseMovement(xpos, ypos);
    
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
    cam->zoomSub(yoffset);
}