#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <Windows.h>
#include "../LearnOpenGL/shader_s.h"

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
	// Run this once and it fixes the problem of the console window showing when running the application
	//ShowWindow(GetConsoleWindow(), false);

	// GLFW: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW: window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program

	Shader shaderProgram_00("simple_vertex.vs", "simple_fragment_00.fs");
	Shader shaderProgram_01("simple_vertex.vs", "simple_fragment_01.fs");
	Shader shaderProgram_02("textured_rectangle_00.vs", "textured_rectangle_00.fs");

	// set up vertex data and buffer(s) and configure vertex attributes
	float triangle_00_vertices[] = {
		// first triangle (left)
		-1.0f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,	// left
		 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,	// right
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f	// top
	};

	float triangle_01_vertices[] = {
		// second triangle (right)
		0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 	// left
		1.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	// right
		0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f	// top
	};

	float rectangle_00_vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int rectangle_00_indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int triangle_00_VAO, triangle_01_VAO, rectangle_00_VAO, triangle_00_VBO, triangle_01_VBO, rectangle_00_VBO, rectangle_00_EBO, texture_00, texture_01;

	glGenVertexArrays(1, &triangle_00_VAO);
	glGenVertexArrays(1, &triangle_01_VAO);
	glGenVertexArrays(1, &rectangle_00_VAO);
	glGenBuffers(1, &triangle_00_VBO);
	glGenBuffers(1, &triangle_01_VBO);
	glGenBuffers(1, &rectangle_00_VBO);
	glGenBuffers(1, &rectangle_00_EBO);
	glGenTextures(1, &texture_00);
	glGenTextures(1, &texture_01);

	glBindVertexArray(triangle_00_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_00_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_00_vertices), triangle_00_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(triangle_01_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_01_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_01_vertices), triangle_01_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(rectangle_00_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectangle_00_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_00_vertices), rectangle_00_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangle_00_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_00_indices), rectangle_00_indices, GL_STATIC_DRAW);
	// Position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture coordinate attribute.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_00);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Resources/Textures/wall.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_01);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Resources/Textures/awesomeface.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	shaderProgram_02.use(); // don't forget to activate the shader before setting uniforms!  
	glUniform1i(glGetUniformLocation(shaderProgram_02.ID, "texture_00"), 0); // set it manually
	shaderProgram_02.setInt("texture_01", 1); // or with shader class

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		shaderProgram_01.use();
		shaderProgram_01.setFloat("ourColor", greenValue);
		glBindVertexArray(triangle_00_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		float horizontalOffsetValue = timeValue * 0.5;
		shaderProgram_00.use();
		shaderProgram_00.setFloat("horizontalOffset", horizontalOffsetValue);
		glBindVertexArray(triangle_01_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		shaderProgram_02.use();
		glBindVertexArray(rectangle_00_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// GLFW: swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// (optional) de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &triangle_00_VAO);
	glDeleteVertexArrays(1, &triangle_01_VAO);
	glDeleteVertexArrays(1, &rectangle_00_VAO);
	glDeleteBuffers(1, &triangle_00_VBO);
	glDeleteBuffers(1, &triangle_01_VBO);
	glDeleteBuffers(1, &rectangle_00_VBO);

	// GLFW: terminate, clearing all previously allocated resources
	glfwTerminate();
	return 0;
}

// process all input this frame (keys pressed, released, etc.)
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// GLFW: whenever the window size changes (OS or user resize) execute this
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}