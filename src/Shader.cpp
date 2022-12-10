#include "Shader.h"
#include "Renderer.h"
#include "MyMath.h"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
  std::string vertexSource = parseShader(vertexPath);
  std::string fragmentSource = parseShader(fragmentPath);

  m_RendererId = createShader(vertexSource, fragmentSource);
}
Shader::~Shader()
{
  GLCall(glDeleteProgram(m_RendererId));
}

std::string Shader::parseShader(const std::string &filepath)
{
  // -- 1 - Retrieve the shader source codes

  std::string shaderSource;
  std::ifstream shaderFile;

  // Ensure that ifstream can throw exceptions
  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    // Open the files
    shaderFile.open(filepath);
    std::stringstream shaderStream;

    // Read file buffer contents into streams
    shaderStream << shaderFile.rdbuf();

    // Close
    shaderFile.close();

    // Convert stream to string
    shaderSource = shaderStream.str();
  }
  catch (std::ifstream::failure &err)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << err.what() << std::endl;
  }

  return shaderSource;
}

unsigned int Shader::compileShader(unsigned int shaderType, const std::string &source)
{
  // -- Compile shaders
  unsigned int shader = glCreateShader(shaderType);
  const char *src = source.c_str();
  GLCall(glShaderSource(shader, 1, &src, NULL));
  GLCall(glCompileShader(shader));

  // Check for errors
  checkCompileErrors(shader, "SHADER", shaderType);

  return shader;
}

unsigned int Shader::createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
  GLCall(unsigned int program = glCreateProgram());

  unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  checkCompileErrors(program, "PROGRAM", 0);

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

void Shader::Bind() const
{
  GLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
  GLCall(glUseProgram(0));
}

void Shader::setBool(const std::string &name, bool value) const
{
  GLCall(glUniform1i(glGetUniformLocation(m_RendererId, name.c_str()), (int)value));
}
void Shader::setUniform1i(const std::string &name, int value) const
{
  GLCall(glUniform1i(glGetUniformLocation(m_RendererId, name.c_str()), value));
}
void Shader::setUniform1f(const std::string &name, float value) const
{
  GLCall(glUniform1f(glGetUniformLocation(m_RendererId, name.c_str()), value));
}

void Shader::setUniform4f(const std::string &name, glm::vec4 values)
{
  GLCall(glUniform4f(getUniformLocation(name), values[0], values[1], values[2], values[3]));
}
void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &matrix)
{
  GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string &name)
{
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];
  GLCall(int location = glGetUniformLocation(m_RendererId, name.c_str()));
  if (location == -1)
  {
    std::cout << "Warning: uniform '" << name << "' doesnt exist." << std::endl;
  }
  m_UniformLocationCache[name] = location;

  return location;
}

void Shader::checkCompileErrors(unsigned int id, std::string type, unsigned int shaderType = 0)
{
  int success;
  char infoLog[1024];

  if (type != "PROGRAM")
  {
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
    if (!success)
    {
      GLCall(glGetShaderInfoLog(id, 1024, NULL, infoLog));
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderType << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
  else
  {
    GLCall(glGetProgramiv(id, GL_LINK_STATUS, &success));
    if (!success)
    {
      GLCall(glGetProgramInfoLog(id, 1024, NULL, infoLog));
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
}