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
    
    bool mouseActive;
    
    float zoomC;
    
    Camera(glm::vec3 iPos, glm::vec3 iFront, glm::vec3 iUp, float iMovSpeed, float iYaw, float iPitch);
    
    Camera();
    
    glm::mat4 getViewMatrix();
    
    //Movements relative to the front-------------------------------------------
    void moveForwarRel(float deltaT);
    
    void moveBackwardRel(float deltaT);
    
    void moveLeftRel(float deltaT);
    
    void moveRightRel(float deltaT);
    //Absolute movements-------------------------------------------------------
    void moveUpAbs(float deltaT);
    
    void moveDownAbs(float deltaT);
    //Mouse event to change front
    void interpretMouseMovement(double x, double y);
    //Scroll event to change zoom in projection
    void zoomSub(double iYoffset);
};



#endif