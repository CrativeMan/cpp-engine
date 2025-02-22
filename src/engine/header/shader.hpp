#ifndef SHADER_HPP
#define SHADER_HPP

#include "../../include/logger.hpp"
#include "fileHandler.hpp"
#include <GL/glew.h>
#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
  unsigned int id;

  Shader(const char *vertexPath, const char *fragmentPath) {
    Logger::debug("Creating shader");
    std::string vertexCode = file::loadStringFromFile(vertexPath);
    if (vertexCode.empty()) {
      Logger::error("Shader", "Failed to load vertex shader");
    }
    const char *vShaderCode = vertexCode.c_str();
    std::string fragmentCode = file::loadStringFromFile(fragmentPath);
    if (fragmentCode.empty()) {
      Logger::error("Shader", "Failed to load fragment shader");
    }
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    Logger::info("Shader", "Vertex shader compiled");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    Logger::info("Shader", "Fragment shader compiled");
    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");
    Logger::info("Shader", "Shader compiled");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  void use() {
    assert(id != 0);
    glUseProgram(id);
  }

  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
  }
  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
  }
  void setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
  }
  void setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(value));
  }

private:
  void checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        Logger::error(
            "Shader",
            "Shader compilation error of type '%s'\n%s\n -- "
            "--------------------------------------------------- -- \n",
            type.c_str());
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        Logger::error(
            "Shader",
            "Program linking error of type '%s'\n%s\n -- "
            "--------------------------------------------------- -- \n",
            type.c_str());
      }
    }
  }
};

#endif // SHADER_HPP
