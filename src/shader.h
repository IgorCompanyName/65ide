#pragma once
#include "GL/glew.h"

typedef struct _vec2 {
    float x, y;
} vec2;

class Shader {
public:
    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();

    void use();
    void uniform2(const char*, vec2);
    void uniform2(const char*, float, float);
private:
    GLuint id;
};