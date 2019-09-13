//
//  Skybox.hpp
//  CGV Project
//
//  Created by Arnold Rebello on 09/05/2019.
//  Copyright © 2019 Arnold Rebello. All rights reserved.
//

#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>


#include </usr/local/Cellar/glm/glm.hpp>
#include </usr/local/Cellar/glm/gtc/matrix_transform.hpp>
#include </usr/local/Cellar/glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Mesh.h"
#include "Shader.h"
class Skybox
{
public:
    Skybox();
    
    Skybox(std::vector<std::string> faceLocations);
    
    void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
    
    ~Skybox();
    
private:
    Mesh* skyMesh;
    Shader* skyShader;
    
    GLuint textureId;
    GLuint uniformProjection, uniformView;
};

