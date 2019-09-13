//
//  Material.h
//  CGV Project
//
//  Created by Arnold Rebello on 02/05/2019.
//  Copyright © 2019 Arnold Rebello. All rights reserved.
//

#pragma once

#include <GL/glew.h>

class Material
{
public:
    Material();
    Material(GLfloat sIntensity, GLfloat shine);
    
    void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
    
    ~Material();
    
private:
    GLfloat specularIntensity;
    GLfloat shininess;
};

