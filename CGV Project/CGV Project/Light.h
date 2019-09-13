//
//  Light.h
//  CGV Project
//
//  Created by Arnold Rebello on 02/05/2019.
//  Copyright © 2019 Arnold Rebello. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include </usr/local/Cellar/glm/glm.hpp>
#include </usr/local/Cellar/glm/gtc/matrix_transform.hpp>
#include "ShadowMap.h"

class Light
{
public:
    Light();
    Light(GLfloat shadowWidth, GLfloat shadowHeight,
          GLfloat red, GLfloat green, GLfloat blue,
          GLfloat aIntensity, GLfloat dIntensity);
    
    ShadowMap* GetShadowMap() { return shadowMap; }
    
    ~Light();
    
protected:
    glm::vec3 colour;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;
    
    glm::mat4 lightProj;
    
    ShadowMap* shadowMap;
};

