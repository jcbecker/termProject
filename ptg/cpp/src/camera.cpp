#include "camera.hpp"


Camera::Camera(glm::vec3 iPos, glm::vec3 iFront, glm::vec3 iUp, float iMovSpeed, float iYaw, float iPitch){
    pos = iPos;
    front = iFront;
    up = iUp;
    movSpeed = iMovSpeed;
    yaw = iYaw;
    pitch = iPitch;
    mouseActive = false;
    zoomC = 45.0f;
}

Camera::Camera(){
    pos = glm::vec3(0.0f, 40.0f,  0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f,  0.0f);
    movSpeed = 8.0;
    yaw = -90.0f;//0.0 let the direction front to the right
    pitch =  0.0f;
    mouseActive = false;
    zoomC = 45.0f;
}

glm::mat4 Camera::getViewMatrix(){
    return glm::lookAt(pos, pos + front, up);
}

//Movements relative to the front-------------------------------------------
void Camera::moveForwarRel(float deltaT){
    this->pos += front * (movSpeed * deltaT);
}
void Camera::moveBackwardRel(float deltaT){
    this->pos -= front * (movSpeed * deltaT);
}
void Camera::moveLeftRel(float deltaT){//Right vecor is equal to glm::normalize(glm::cross(cameraFront, cameraUp))
    this->pos -= glm::normalize(glm::cross(front, up)) * (movSpeed * deltaT);
}
void Camera::moveRightRel(float deltaT){
    this->pos += glm::normalize(glm::cross(front, up)) * (movSpeed * deltaT);
}
void Camera::moveForwarRelXZ(float deltaT){
    float auxy = this->pos.y;
    this->pos += front * (movSpeed * deltaT);
    this->pos.y = auxy;
}
void Camera::moveBackwardRelXZ(float deltaT){
    float auxy = this->pos.y;
    this->pos -= front * (movSpeed * deltaT);
    this->pos.y = auxy;
}
void Camera::moveLeftRelXZ(float deltaT){//Right vecor is equal to glm::normalize(glm::cross(cameraFront, cameraUp))
    float auxy = this->pos.y;
    this->pos -= glm::normalize(glm::cross(front, up)) * (movSpeed * deltaT);
    this->pos.y = auxy;
}
void Camera::moveRightRelXZ(float deltaT){
    float auxy = this->pos.y;
    this->pos += glm::normalize(glm::cross(front, up)) * (movSpeed * deltaT);
    this->pos.y = auxy;
}


//Absolute movements-------------------------------------------------------
void Camera::moveUpAbs(float deltaT){
    this->pos += glm::vec3( 0.0f, (movSpeed * deltaT),  0.0f);
}
void Camera::moveDownAbs(float deltaT){
    this->pos -= glm::vec3( 0.0f, (movSpeed * deltaT),  0.0f);
}
//Mouse event to change front
void Camera::interpretMouseMovement(double x, double y){
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
//Scroll event to change zoom in projection
void Camera::zoomSub(double iYoffset){
    zoomC -= iYoffset;
    if(zoomC > 45)
        zoomC = 45;
    else if(zoomC < 1)
        zoomC = 1;
}

