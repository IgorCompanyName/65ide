#pragma once
#include "window.h"
#include "GL/glew.h"
#include <GLFW/glfw3.h>

class Events {
public:
    Events(GLFWwindow*);
    bool getKeyPress(int keycode);
    bool getKeyRelease(int keycode);
    void pollEvents();
private:
    GLFWwindow* window;
};