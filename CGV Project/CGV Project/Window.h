//
//  Window.h
//  CGV Project
//
//  Created by Arnold Rebello on 02/05/2019.
//  Copyright © 2019 Arnold Rebello. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();
    
    Window(GLint windowWidth, GLint windowHeight);
    
    int Initialise();
    
    GLint getBufferWidth() { return bufferWidth; }
    GLint getBufferHeight() { return bufferHeight; }
    
    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
    bool* getsKeys() { return keys; }
    GLfloat getXChange();
    GLfloat getYChange();
    void swapBuffers() { glfwSwapBuffers(mainWindow); }
    
    ~Window();
    
private:
    GLFWwindow* mainWindow;
    
    GLint width, height;
    GLint bufferWidth, bufferHeight;
    bool keys[1024];
    
    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved;
    
    void createCallbacks();
   static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};
