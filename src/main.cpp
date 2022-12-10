#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MyMath.h"

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

// Whenever window size changes, GLFW calls this function and fills in the proper arguments.
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  GLCall(glViewport(0, 0, width, height));
}

void processInput(GLFWwindow *window)
{
}

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "WINDOWLOL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Register the function
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Shader program
  Shader shaderProgram("src/shaders/shader.vertex", "src/shaders/shader.frag");

  float vertices[] = {
      // Positions	      // Colors			    // Texture
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
  };

  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3};

  unsigned int VAO;
  GLCall(glGenVertexArrays(1, &VAO));
  GLCall(glBindVertexArray(VAO));

  VertexArray va;
  VertexBuffer vb(vertices, sizeof(vertices));

  VertexBufferLayout layout;
  layout.Push<float>(3);
  layout.Push<float>(3);
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 6);

  // Pos
  GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0));
  GLCall(glEnableVertexAttribArray(0));
  // Colors
  GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(3 * sizeof(float))));
  GLCall(glEnableVertexAttribArray(1));
  // Tex coords
  GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(6 * sizeof(float))));
  GLCall(glEnableVertexAttribArray(2));

  shaderProgram.Bind();
  // --- Textures

  Texture texture1("C:/Users/Leevi/Downloads/container.jpg");

  shaderProgram.setUniform1i("texture1", 0);

  Renderer renderer;
  // Render loop
  while (!glfwWindowShouldClose(window))
  {
    renderer.Clear();
    processInput(window);

    texture1.Bind(0);

    // --- 1. Transformation
    glm::mat4 trans = glm::mat4(1.0f);                                             // Identity matrix
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));                   // Move to bottom right
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate

    shaderProgram.Bind();
    shaderProgram.setUniformMat4f("transform", trans);

    renderer.Draw(va, ib, shaderProgram);

    // Swaps the color buffer that is used to render to during this render iteration.
    glfwSwapBuffers(window);
    // Check if any events are triggered, updates the window state,
    // and calls the corresponding functions.
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
