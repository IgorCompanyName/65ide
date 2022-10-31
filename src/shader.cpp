#include "shader.h"
#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <string.h>

std::string readFile(const char *filePath) {
    using namespace std;

    string content;
    ifstream file(filePath, ios::in);

    if(!file.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }
    string line = "";
    while(!file.eof()) {
        getline(file, line);
        content.append(line + "\n");
    }
    file.close();
    return content;
}

Shader::Shader(const char* vSource, const char* fSource) {
    std::string vertex = readFile(vSource);
    std::string fragment = readFile(fSource);
    const char* vertexSource = vertex.c_str();
    const char* fragmentSource = fragment.c_str();

    GLuint vid = glCreateShader(GL_VERTEX_SHADER);
    GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vid, 1, &vertexSource, NULL);
    glCompileShader(vid);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vid, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vid, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glShaderSource(fid, 1, &fragmentSource, NULL);
    glCompileShader(fid);
    glGetShaderiv(fid, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fid, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint program;
    program = glCreateProgram();

    glAttachShader(program, vid);
    glAttachShader(program, fid);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
    id = program;
    glDeleteShader(vid);
    glDeleteShader(fid);
}
void Shader::use() {
    glUseProgram(id);
}
Shader::~Shader() {
    glDeleteProgram(id);
}
void Shader::uniform2(const char* name, vec2 v) {
    int loc = glGetUniformLocation(id, name);
    glUniform2f(loc, v.x, v.y);
}
void Shader::uniform2(const char* name, float v0, float v1) {
    int loc = glGetUniformLocation(id, name);
    glUniform2f(loc, v0, v1);
}