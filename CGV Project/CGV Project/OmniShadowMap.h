//
//  OmniShadowMap.hpp
//  CGV Project
//
//  Created by Arnold Rebello on 09/05/2019.
//  Copyright © 2019 Arnold Rebello. All rights reserved.
//

#pragma once
#include "ShadowMap.h"
class OmniShadowMap :
public ShadowMap
{
public:
    OmniShadowMap();
    
    bool Init(unsigned int width, unsigned int height);
    
    void Write();
    
    void Read(GLenum TextureUnit);
    
    ~OmniShadowMap();
};

