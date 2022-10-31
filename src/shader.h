#pragma once
#include "GL/glew.h"

class Shader {
public:
    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();

    void use();
private:
    GLuint id;
};