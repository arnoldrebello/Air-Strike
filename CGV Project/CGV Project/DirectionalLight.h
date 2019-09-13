//
//  DirectionalLight.h
//  CGV Project
//
//  Created by Arnold Rebello on 02/05/2019.
//  Copyright © 2019 Arnold Rebello. All rights reserved.
//

#pragma once
#include "Light.h"

class DirectionalLight :
public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
                     GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat aIntensity, GLfloat dIntensity,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir);
    
    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
                  GLfloat diffuseIntensityLocation, GLfloat directionLocation);
    
    glm::mat4 CalculateLightTransform();
    
    ~DirectionalLight();
    
private:
    glm::vec3 direction;
};
