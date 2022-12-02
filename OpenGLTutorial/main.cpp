#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"

// Whenever window size changes, GLFW calls this function and fills in the proper arguments.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "WINDOWLOL", NULL, NULL);
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
	

	Shader shaderProgram("shader.vs", "shader.fs");

	float vertices[] = {
		// Positions	   // Colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // left
	   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // right
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // top 
	};

	unsigned int VAO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// --- 1. TRIANGLE ------------------ 
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Render loop
	float xPos = -0.5f;
	char dir = 'r';
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();

		if (dir == 'l')
		{
			if (xPos <= -0.5f)
			{
				dir = 'r';
			}
			else {
				xPos -= 0.005f;
			}
		}
		if (dir == 'r')
		{
			if (xPos >= 0.5f)
			{
				dir = 'l';
			}
			else {
				xPos += 0.005f;
			}
		}

		shaderProgram.setFloat("horizontalPos", xPos);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		// Swaps the color buffer that is used to render to during this render iteration.
		glfwSwapBuffers(window);
		// Check if any events are triggered, updates the window state, 
		// and calls the corresponding functions.
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

