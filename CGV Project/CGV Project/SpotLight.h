//
//  SpotLight.h
//  CGV Project
//
//  Created by Arnold Rebello on 03/05/2019.
//  Copyright © 2019 Arnold Rebello. All rights reserved.
//

#pragma once
#include "PointLight.h"
class SpotLight :
public PointLight
{
public:
    SpotLight();
    
    SpotLight(GLfloat shadowWidth, GLfloat shadowHeight,
              GLfloat near, GLfloat far,
              GLfloat red, GLfloat green, GLfloat blue,
              GLfloat aIntensity, GLfloat dIntensity,
              GLfloat xPos, GLfloat yPos, GLfloat zPos,
              GLfloat xDir, GLfloat yDir, GLfloat zDir,
              GLfloat con, GLfloat lin, GLfloat exp,
              GLfloat edg);
    
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                  GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
                  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
                  GLuint edgeLocation);
    
    void SetFlash(glm::vec3 pos, glm::vec3 dir);
    
    void Toggle() { isOn = !isOn; }
    
    ~SpotLight();
    
private:
    glm::vec3 direction;
    
    GLfloat edge, procEdge;
    
    bool isOn;
};

