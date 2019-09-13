//
//  Camera.h
//  CGV Project
//
//  Created by Arnold Rebello on 02/05/2019.
//  Copyright © 2019 Arnold Rebello. All rights reserved.
//
#pragma once
#include <GL/glew.h>
#include </usr/local/Cellar/glm/glm.hpp>
#include </usr/local/Cellar/glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
class Camera
{
public:
    Camera();
    Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
    
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    
    glm::vec3 getCameraPosition();
    glm::vec3 getCameraDirection();
    
    glm::mat4 calculateViewMatrix();
    
    ~Camera();
    
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    GLfloat yaw;
    GLfloat pitch;
    
    GLfloat moveSpeed;
    GLfloat turnSpeed;
    
    void update();
};
