//
//  Material.cpp
//  CGV Project
//
//  Created by Arnold Rebello on 02/05/2019.
//  Copyright © 2019 Arnold Rebello. All rights reserved.
//

#include "Material.h"

Material::Material()
{
    specularIntensity = 0.0f;
    shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
    specularIntensity = sIntensity;
    shininess = shine;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
}

Material::~Material()
{
}

