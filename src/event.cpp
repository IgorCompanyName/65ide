#include "event.h"
#include <GLFW/glfw3.h>

Events::Events(GLFWwindow* w) {
    window = w;
}
void Events::pollEvents() {
    glfwPollEvents();
}
bool Events::getKeyPress(int keycode) {
    return glfwGetKey(window, keycode) == GLFW_PRESS;
}