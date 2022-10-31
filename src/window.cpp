#include "window.h"
#include <iostream>

Window::Window(int w, int h, const char* t) {
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(w, h, t, nullptr, nullptr);
    if(!window) { // idk, maybe put glfwDestroyWindow here. i hope not
        window = nullptr;
        std::cerr << "Failed to create window" << std::endl;
        return;
    }
    glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        glfwDestroyWindow(window);
        window = nullptr;
        return;
    }
    glViewport(0, 0, w, h);
}
bool Window::isNull() {
    return window == nullptr;
}
Window::~Window() {
    terminate();
}
void Window::terminate() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
void Window::swapBuffers() {
    glfwSwapBuffers(window);
}
bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
}
void Window::setShouldClose(bool v) {
    glfwSetWindowShouldClose(window, v);
}