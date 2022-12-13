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
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 960.0f / 2.0f;
float lastY = 540.0f / 2.0f;
float fov = 45.0f;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void processInput(GLFWwindow *window);

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

  // Register functions
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  // Capture cursor
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  // glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -200.0f));

  glm::vec3 translationA(200, 200, 0);
  glm::vec3 translationB(400, 200, 0);

  // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
  // glm::vec3 cameraDirection = glm::normalize(cameraPos-cameraTarget);

  // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
  // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

  // glm::mat4 view;
  // view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
  //                    glm::vec3(0.0f, 0.0f, 0.0f),
  //                    glm::vec3(0.0f, 1.0f, 3.0f));

  glEnable(GL_DEPTH_TEST);
  // Render loop
  while (!glfwWindowShouldClose(window))
  {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  
    renderer.Clear();
    processInput(window);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    texture1.Bind(0);

    shaderProgram.Bind();
    glm::mat4 proj = glm::perspective(glm::radians(fov), 960.0f / 540.0f, 0.1f, 500.0f);

    // --- 1. Transformation
    glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    float radius = 300.0f;
    float camX = static_cast<float>(sin(glfwGetTime()) * radius);
    float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    {

      glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
      model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      shaderProgram.Bind();

      
      glm::mat4 mvp = proj * view * model;

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

void processInput(GLFWwindow *window)
{
  const float cameraSpeed = 100.0f * deltaTime; // adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f; // change this value to your liking
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (pitch > 89.0f)
      pitch = 89.0f;
  if (pitch < -89.0f)
      pitch = -89.0f;
  glm::vec3 front;

  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  fov -= (float)yoffset;
  if (fov < 1.0f)
      fov = 1.0f;
  if (fov > 45.0f)
      fov = 45.0f;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  GLCall(glViewport(0, 0, width, height));
}