#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    
    float movSpeed;
    float yaw;
    float pitch;
    
    double lastMposx, lastMposy;
    double xpost, ypost;
    
    bool mouseActive;
    
    Camera(glm::vec3 iPos, glm::vec3 iFront, glm::vec3 iUp, float iMovSpeed, float iYaw, float iPitch){
        pos = iPos;
        front = iFront;
        up = iUp;
        movSpeed = iMovSpeed;
        yaw = iYaw;
        pitch = iPitch;
        mouseActive=false;
    }
    
    Camera(){
        pos = glm::vec3(0.0f, 0.0f,  3.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f,  0.0f);
        movSpeed = 2.5;
        yaw = -90.0f;//0.0 let the direction front to the right
        pitch =  0.0f;
        
        xpost=-1;
        ypost=-1;
        mouseActive=false;
    }
    
    glm::mat4 getViewMatrix(){
        return glm::lookAt(pos, pos + front, up);
    }
    
    //Movements relative to the front-------------------------------------------
    void moveForwarRel(float deltaT){
        pos += front * (movSpeed * deltaT);
    }
    void moveBackwardRel(float deltaT){
        pos -= front * (movSpeed * deltaT);
    }
    void moveLeftRel(float deltaT){//Right vecor is equal to glm::normalize(glm::cross(cameraFront, cameraUp))
        pos -= glm::normalize(glm::cross(front, up)) * (movSpeed * deltaT);
    }
    void moveRightRel(float deltaT){
        pos += glm::normalize(glm::cross(front, up)) * (movSpeed * deltaT);
    }
    //Absolute movements-------------------------------------------------------
    void moveUpAbs(float deltaT){
        pos += glm::vec3( 0.0f, (movSpeed * deltaT),  0.0f);
    }
    void moveDownAbs(float deltaT){
        pos -= glm::vec3( 0.0f, (movSpeed * deltaT),  0.0f);
    }
    //Mouse event to change front
    void interpretMouseMovement(double x, double y){
        if(!mouseActive){
            lastMposx=x;
            lastMposy=y;
            mouseActive=true;
        }else{
            float xoffset = x - lastMposx;
            float yoffset = lastMposy - y; // reversed since y-coordinates range from bottom to top
            lastMposx=x;
            lastMposy=y;
            
            float sensitivity = 0.05f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;
            
            yaw += xoffset;
            pitch += yoffset;
            
            if(pitch > 89.0f)
                pitch = 89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;
        
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            front = glm::normalize(front);
        }
    }
    
};

#endif