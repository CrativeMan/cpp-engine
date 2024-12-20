#ifndef SHADER_H
#define SHADER_H

#include "../include/logger.hpp"
#include "fileHandler.hpp"
#include <GL/glew.h>
#include <string>

class Shader {
public:
  unsigned int id;

  Shader(const char *vertexPath, const char *fragmentPath) {
    const char *vertexShaderSource = FileHandler::readFile(vertexPath);
    const char *fragmentShaderSource = FileHandler::readFile(fragmentPath);
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex, 512, NULL, infoLog);
      Logger::warn("VERTEX", "Shader compilation failed\n");
      Logger::warn("VERTEX", infoLog);
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment, 512, NULL, infoLog);
      Logger::warn("FRAGMENT", "Shader compilation failed\n");
      Logger::warn("FRAGMENT", infoLog);
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      Logger::warn("SHADER", "Shader compilation failed\n");
      Logger::warn("SHADER", infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  void use() { glUseProgram(id); }

  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
  }
  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
  }
};

#endif // SHADER_H
