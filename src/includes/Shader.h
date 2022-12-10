#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "MyMath.h"
#include <unordered_map>

class Shader
{
private:
  unsigned int m_RendererId;
  std::unordered_map<std::string, int> m_UniformLocationCache;

public:
  // Constructor reads and builds the shader
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  ~Shader();

  void Bind() const;
  void Unbind() const;
  // Utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setUniform1i(const std::string &name, int value) const;
  void setUniform1f(const std::string &name, float value) const;
  void setUniform4f(const std::string &name, glm::vec4 values);
  void setUniformMat4f(const std::string &name, const glm::mat4 &values);

  inline unsigned int GetId() const { return m_RendererId; }

private:
  void checkCompileErrors(unsigned int id, std::string type, unsigned int shaderType);
  std::string parseShader(const std::string &filepath);
  unsigned int compileShader(unsigned int type, const std::string &source);
  unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader);
  int getUniformLocation(const std::string &name);
};
