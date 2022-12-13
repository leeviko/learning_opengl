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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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

  GLFWwindow *window = glfwCreateWindow(960, 540, "WINDOWLOL", NULL, NULL);
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
      -50.0f, -50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      50.0f, -50.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
      50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // bottom left
      -50.0f, 50.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
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

  glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

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
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
  ImGui::StyleColorsClassic();

  glm::vec3 translationA(200, 200, 0);
  glm::vec3 translationB(400, 200, 0);

  // Render loop
  while (!glfwWindowShouldClose(window))
  {
    renderer.Clear();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    texture1.Bind(0);

    // --- 1. Transformation
    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
      glm::mat4 mvp = proj * view * model;
      shaderProgram.Bind();
      shaderProgram.setUniformMat4f("u_MVP", mvp);
      renderer.Draw(va, ib, shaderProgram);
    }
    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
      glm::mat4 mvp = proj * view * model;
      shaderProgram.setUniformMat4f("u_MVP", mvp);
      renderer.Draw(va, ib, shaderProgram);
    }
    {
      ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
      ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // Swaps the color buffer that is used to render to during this render iteration.
    glfwSwapBuffers(window);
    // Check if any events are triggered, updates the window state,
    // and calls the corresponding functions.
    glfwPollEvents();
  }
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}
