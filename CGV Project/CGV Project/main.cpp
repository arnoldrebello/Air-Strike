#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include "Skybox.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include </usr/local/Cellar/glm/glm.hpp>
#include </usr/local/Cellar/glm/gtc/matrix_transform.hpp>
#include </usr/local/Cellar/glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "ShadowMap.h"
#include "Model.h"

const float toRadians = 3.14159265f / 180.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.002f;
GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0,
uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0,uniformFarPlane = 0;

Window mainWindow;
std::vector<Mesh*> meshList;

std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture grassTexture;

Material shinyMaterial;
Material dullMaterial;

Model xwing;
Model blackhawk;


DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Skybox skybox;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLfloat blackhawkAngle = 0.0f;
GLfloat xwingHeight = 0.0f;
// Vertex Shader
static const char* vShader = "/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Shaders/shader.frag";

void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
                        unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);
        
        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }
    
    for (size_t i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };
    
    GLfloat vertices[] = {
        //    x      y      z            u      v            nx      ny    nz
        -1.0f, -1.0f, -0.6f,        0.0f, 0.0f,        0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f,        0.5f, 0.0f,        0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -0.6f,        1.0f, 0.0f,        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,        0.5f, 1.0f,        0.0f, 0.0f, 0.0f
    };
    
    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };
    
    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,    0.0f, 0.0f,        0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,    10.0f, 0.0f,    0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f,        10.0f, 10.0f,    0.0f, -1.0f, 0.0f
    };
    
    calcAverageNormals(indices, 12, vertices, 32, 8, 5);
    
    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);
    
    Mesh *obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj2);
    
    Mesh *obj3 = new Mesh();
    obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(obj3);
}

void CreateShaders()
{
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
    
    
    
    directionalShadowShader = Shader();
    directionalShadowShader.CreateFromFiles("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Shaders/directional_shadow_map.vert", "/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Shaders/directional_shadow_map.frag");
    omniShadowShader.CreateFromFiles("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Shaders/omni_directional_shadow_map.vert", "/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Shaders/omni_directional_shadow_map.geom", "/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Shaders/omni_directional_shadow_map.frag");
}

void RenderScene()
{
    glm::mat4 model(1.0f);
    
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.UseTexture();
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->RenderMesh();
    if (direction)
    {
        triOffset += triIncrement;
    }
    else {
        triOffset -= triIncrement;
    }
    
    if (abs(triOffset) >= triMaxOffset)
    {
        direction = !direction;
    }
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
    model = glm::scale(model, glm::vec3(triOffset, triOffset, triOffset));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[1]->RenderMesh();
    
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[2]->RenderMesh();
    if (direction)
    {
        triOffset += triIncrement;
    }
    else {
        triOffset -= triIncrement;
    }
    
    if (abs(triOffset) >= triMaxOffset)
    {
        direction = !direction;
    }
    
   /* model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-7.0f, triOffset, 10.0f));
    model = glm::scale(model, glm::vec3(0.006f, 0.006, 0.006f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    xwing.RenderModel();*/
    
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, triOffset+5, 0.0f));
    model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    blackhawk.RenderModel();
    blackhawkAngle += 0.1f;
    if (blackhawkAngle > 360.0f)
    {
        blackhawkAngle = 0.1f;
    }
    
    model = glm::mat4(1.0f);
    model = glm::rotate(model, ((-blackhawkAngle * toRadians)), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, 5.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(4.0f, 5.0f, 4.0f));
    //model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    
    model = glm::scale(model, glm::vec3(0.006f, 0.006, 0.006f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    xwing.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
    directionalShadowShader.UseShader();
    
    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
    
    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);
    glm::mat4 conversion1 = light->CalculateLightTransform();
    uniformModel = directionalShadowShader.GetModelLocation();
   directionalShadowShader.SetDirectionalLightTransform(&conversion1);
    
    RenderScene();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
    
    omniShadowShader.UseShader();
    uniformModel = omniShadowShader.GetModelLocation();
    uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
    uniformFarPlane = omniShadowShader.GetFarPlaneLocation();
    
    light->GetShadowMap()->Write();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
    glUniform1f(uniformFarPlane, light->GetFarPlane());
    omniShadowShader.SetOmniLightMatrices(light->CalculateLightTransform());
    
    omniShadowShader.Validate();
    RenderScene();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{   glViewport(00, 00, 3200, 2000);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    skybox.DrawSkybox(viewMatrix,projectionMatrix);
    
    shaderList[0].UseShader();
    
    uniformModel = shaderList[0].GetModelLocation();
    uniformProjection = shaderList[0].GetProjectionLocation();
    uniformView = shaderList[0].GetViewLocation();
    uniformModel = shaderList[0].GetModelLocation();
    uniformEyePosition = shaderList[0].GetEyePositionLocation();
    uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
    uniformShininess = shaderList[0].GetShininessLocation();
    
   
    
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
    
    shaderList[0].SetDirectionalLight(&mainLight);
    glm::mat4 conversion2  =mainLight.CalculateLightTransform();
    shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
    shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
    shaderList[0].SetDirectionalLightTransform(&conversion2);
    
    mainLight.GetShadowMap()->Read(GL_TEXTURE1);
    shaderList[0].SetTexture(0);
    shaderList[0].SetDirectionalShadowMap(1);
    
    glm::vec3 lowerLight = camera.getCameraPosition();
    lowerLight.y -= 0.3f;
    //spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
    
    RenderScene();
}

int main()
{
    mainWindow = Window(1440, 900); // 1280, 1024 or 1024, 768
    mainWindow.Initialise();
    
    CreateObjects();
    CreateShaders();
    
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
    
    brickTexture = Texture("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Textures/brick.png");
    brickTexture.LoadTextureA();
    dirtTexture = Texture("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Textures/dirt.png");
    dirtTexture.LoadTextureA();
    plainTexture = Texture("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Textures/plain.png");
    plainTexture.LoadTextureA();
    grassTexture = Texture("/Users/arnoldrebello/Desktop/IMG_2077.png");
    grassTexture.LoadTextureA();
    
    shinyMaterial = Material(4.0f, 256);
    dullMaterial = Material(0.3f, 4);
    
    xwing = Model();
    xwing.LoadModel("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Models/x-wing.obj");
    
    blackhawk = Model();
    blackhawk.LoadModel("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Models/uh60.obj");
    
    
    
    mainLight = DirectionalLight(2048, 2048,
                                 1.0f, 1.0f, 1.0f,
                                 0.0f, 0.0f,
                                 0.0f, -15.0f, -10.0f);
    
    pointLights[1] = PointLight(1024, 1024,
                                0.1f, 100.0f,
                                0.0f, 0.0f, 1.0f,
                                0.0f, 0.4f,
                                2.0f, 2.0f, 0.0f,
                                0.3f, 0.01f, 0.01f);
    pointLightCount++;
    pointLights[0] = PointLight(1024, 1024,
                                0.1f, 100.0f,
                                0.0f, 1.0f, 0.0f,
                                0.0f, 0.4f,
                                -2.0f, 2.0f, 0.0f,
                                0.3f, 0.01f, 0.01f);
    pointLightCount++;
    
    
    spotLights[0] = SpotLight(1024, 1024,
                              0.1f, 100.0f,
                              1.0f, 1.0f, 1.0f,
                              0.0f, 2.0f,
                              0.0f, 0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              20.0f);
    spotLightCount++;
    spotLights[1] = SpotLight(1024, 1024,
                              0.1f, 100.0f,
                              1.0f, 1.0f, 1.0f,
                              0.0f, 1.0f,
                              0.0f, -1.5f, 0.0f,
                               -100.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              20.0f);
    spotLightCount++;
    std::vector<std::string> skyboxFaces;
    skyboxFaces.push_back("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Textures/Skybox/cupertin-lake_rt.tga");
    skyboxFaces.push_back("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Textures/Skybox/cupertin-lake_lf.tga");
    skyboxFaces.push_back("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Textures/Skybox/cupertin-lake_up.tga");
    skyboxFaces.push_back("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Textures/Skybox/cupertin-lake_dn.tga");
    skyboxFaces.push_back("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Textures/Skybox/cupertin-lake_bk.tga");
    skyboxFaces.push_back("/Users/arnoldrebello/Library/Mobile Documents/com~apple~CloudDocs/Programming Languages/Other Programming languages/CGV Project/CGV Project/Textures/Skybox/cupertin-lake_ft.tga");
    
    skybox = Skybox(skyboxFaces);
    
    

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
    
    // Loop until window closed
    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
        deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
        lastTime = now;
        
        // Get + Handle User Input
        glfwPollEvents();
        
        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
        
        DirectionalShadowMapPass(&mainLight);
        RenderPass(camera.calculateViewMatrix(), projection);
        
        mainWindow.swapBuffers();
    }
    
    return 0;
}
