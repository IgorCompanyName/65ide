#pragma once
#include "window.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Events {
public:
    Events(GLFWwindow*);
    bool getKeyPress(int keycode);
    void pollEvents();
private:
    GLFWwindow* window;
};