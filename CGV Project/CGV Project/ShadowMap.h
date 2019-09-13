//
//  ShadowMap.h
//  CGV Project
//
//  Created by Arnold Rebello on 07/05/2019.
//  Copyright © 2019 Arnold Rebello. All rights reserved.
//
#pragma once
#include <stdio.h>
#include <GL/glew.h>

class ShadowMap
{
public:
    ShadowMap();
    
    virtual bool Init(GLuint width, GLuint height);
    
    virtual void Write();
    
    virtual void Read(GLenum textureUnit);
    
    GLuint GetShadowWidth() { return shadowWidth; }
    GLuint GetShadowHeight() { return shadowHeight; }
    
    ~ShadowMap();
    
protected:
    GLuint FBO, shadowMap;
    GLuint shadowWidth, shadowHeight;
};



