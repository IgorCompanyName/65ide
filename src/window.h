#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    bool isNull();
    bool shouldClose();
    void setShouldClose(bool);
    void swapBuffers();
    void terminate();

    GLFWwindow* window = nullptr;
};