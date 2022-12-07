#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
  // Program id
  unsigned int id;

  // Constructor reads and builds the shader
  Shader(const char *vertexPath, const char *fragmentPath);

  // Use/activate the shader
  void use();

  // Utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;

private:
  void checkCompileErrors(unsigned int shader, std::string type);
};
