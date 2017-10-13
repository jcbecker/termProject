//Author: João C. Becker

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>
#include <camera.hpp>
//Global settings
//initial screen size
const unsigned int ISCR_W = 80 * 16;//WIDTH
const unsigned int ISCR_H = 80 * 9;//HEIGHT


static void errorCallback(int error, const char* description);//to show some glfw errors

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);//input callBack

void framebufferSizeCallback(GLFWwindow* window, int width, int height);//resize screen

void processInputHoldingKey(GLFWwindow *window, float deltaTime);//to manage keys holding down

static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);//send mouse position to the camera

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

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
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//this put de cursor in camera mode
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Failed to initialize GLAD");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // configure global opengl state to use z-buffer
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    
    Shader ourShader("shader33Vertex.vs", "shader33Fragment.fs"); // you can name your shader files however you like
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // colors
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,// top right
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,// bottom right
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,// bottom left
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,// top left
        
        0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 0.0f,// top right
        0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f,// bottom right
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,// bottom left
        -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 0.0f,// top left
        
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,// top left
        -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,// top left
        0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f,// top right
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,// top right
        
        -0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,// bottom left
        -0.5f,  -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,// bottom left
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,// bottom right
        0.5f,  -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,// bottom right
        
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,// top right
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,// bottom right
        0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,// bottom right
        0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,// bottom right
        
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,// top right
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,// bottom right
        -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f,// bottom right
        -0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 0.0f// bottom right
        
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 3, 2,  // first Triangle
        1, 2, 0,   // second Triangle
        
        0+4, 3+4, 2+4,  // first Triangle
        1+4, 2+4, 0+4,   // second Triangle
        
        0+8, 3+8, 2+8,  // first Triangle
        1+8, 2+8, 0+8,   // second Triangle
        
        0+(4*3), 3+(4*3), 2+(4*3),  // first Triangle
        1+(4*3), 2+(4*3), 0+(4*3),   // second Triangle
        
        0+(4*4), 3+(4*4), 2+(4*4),  // first Triangle
        1+(4*4), 2+(4*4), 0+(4*4),   // second Triangle
        
        0+(4*5), 3+(4*5), 2+(4*5),  // first Triangle
        1+(4*5), 2+(4*5), 0+(4*5)  // second Triangle
    };
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    
    
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
    
    printf("VBO: %u   VAO: %u   EBO: %u   aVBO: %u   aVAO: %u\n", VBO, VAO, EBO, aVBO, aVAO);
    printf("vertices: %d   indices: %d    myAxes: %d\n", sizeof(vertices), 12*3*sizeof(unsigned int), 6*6*sizeof(float));
    

    
    

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glViewport(0, 0, ISCR_W, ISCR_H);
    
    //creating variables
    
    int cscr_w=ISCR_W, cscr_h=ISCR_H;
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;
    
    Camera cam;
    glfwSetWindowUserPointer(window, &cam);
    
    int myi=0;
    //mainLoop
    printf("Entering in the main loop...\n");
    while (!glfwWindowShouldClose(window)){
        float cFrameTime = glfwGetTime();
        deltaTime = cFrameTime - lastFrame;
        lastFrame = cFrameTime;
        
        
        printf("\r%d  %lf (%lf, %lf, %lf)  (%lf)",myi++, glfwGetTime(), cam.pos.x, cam.pos.y, cam.pos.z, cam.zoomC);
        fflush(stdout);
        
        //process holding keys
        processInputHoldingKey(window, deltaTime);
        
        //render
        //clear the collor buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
        
        
        //active the shader and bind uniforms
        ourShader.use();
        
        // create transformations
        view  = cam.getViewMatrix();
        glfwGetWindowSize(window, &cscr_w, &cscr_h);//essa função é threadsafe
        projection = glm::perspective(glm::radians(cam.zoomC), (float)cscr_w / (float)cscr_h, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        
        //render container
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        for(unsigned int i = 0; i < 10; i++){
            
            
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 30.0f * i;
            if (i%2){
                model = glm::rotate(model, glm::radians((float)glfwGetTime()*100), glm::vec3(0.0f, 0.0f, 1.0f));
            }else{
                model = glm::rotate(model, glm::radians((float)glfwGetTime()*100), glm::vec3(-1.0f, 0.0f, 0.0f));
            }
            
            ourShader.setMat4("model", model);
            
            glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
        }
        
        glBindVertexArray(aVAO);
        model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);
        
        glDrawArrays(GL_LINES, 0, 6);
        //glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // no need to unbind it every time
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    printf("\n");
    
    printf("Free memory space...\n");
    //de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

static void errorCallback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){//input callBack
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height){//resize screen
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    printf("Screen:(%d, %d)\n",width, height);
}

void processInputHoldingKey(GLFWwindow *window, float deltaTime){
    Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->moveForwarRel(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->moveBackwardRel(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->moveLeftRel(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->moveRightRel(deltaTime);
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