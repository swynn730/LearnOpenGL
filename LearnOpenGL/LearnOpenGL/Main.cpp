#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <Windows.h>
#include "../LearnOpenGL/shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../LearnOpenGL/camera.h"

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float mixerValue = 0.0;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

/*
float yaw = 0.0f;
float pitch = 0.0f;
bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;
float fov = 45.0f;
*/

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

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


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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
	Shader shaderProgram_03("textured_rectangle_01.vs", "textured_rectangle_00.fs");

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
			// positions			// colors				// texture coords
			0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,	// top right
			0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,	// bottom right
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,	// bottom left
			-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f	// top left 
	};

	float cube_00_vertices[] = {
			// positions			// colors				// texture coords
			-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f,		1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f
	};

	unsigned int rectangle_00_indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int triangle_00_VAO, triangle_01_VAO, rectangle_00_VAO;
	unsigned int triangle_00_VBO, triangle_01_VBO, rectangle_00_VBO;
	unsigned int rectangle_00_EBO;
	unsigned int texture_00, texture_01;

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(rectangle_00_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectangle_00_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_00_vertices), cube_00_vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangle_00_EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_00_indices), rectangle_00_indices, GL_STATIC_DRAW);

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
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
	//GL_LINEAR_MIPMAP_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Resources/Textures/awesomeface.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

	shaderProgram_03.use(); // don't forget to activate the shader before setting uniforms!  
	glUniform1i(glGetUniformLocation(shaderProgram_03.ID, "texture_00"), 0); // set it manually
	shaderProgram_03.setInt("texture_00", 1); // or with shader class

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		shaderProgram_01.use();
		shaderProgram_01.setFloat("ourColor", greenValue);
		glBindVertexArray(triangle_00_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		float horizontalOffsetValue = timeValue * 0.1;
		shaderProgram_00.use();
		shaderProgram_00.setFloat("horizontalOffset", horizontalOffsetValue);
		glBindVertexArray(triangle_01_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		shaderProgram_02.use();
		//glm::mat4 rectangle_00_trans = glm::mat4(1.0f);
		//rectangle_00_trans = glm::translate(rectangle_00_trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//rectangle_00_trans = glm::rotate(rectangle_00_trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		

		/*
		glm::mat4 view;
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);		

		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		*/

		/*
		glm::vec3 direction;
		direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		*/

		glm::mat4 view = camera.GetViewMatrix();

		//glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

		//glUniformMatrix4fv(glGetUniformLocation(shaderProgram_02.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram_02.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram_02.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//glUniformMatrix4fv(glGetUniformLocation(shaderProgram_02.ID, "transform"), 1, GL_FALSE, glm::value_ptr(rectangle_00_trans));
		//shaderProgram_02.setFloat("mixerValue", mixerValue);
		shaderProgram_02.setFloat("texCoordScale", 2.0f);
		glBindVertexArray(rectangle_00_VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (i == 0 || i % 3 == 0) 
			{
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else 
			{
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram_02.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		shaderProgram_03.use();
		glm::mat4 rectangle_01_trans = glm::mat4(1.0f);
		rectangle_01_trans = glm::translate(rectangle_01_trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		rectangle_01_trans = glm::scale(rectangle_01_trans, abs(sin((float)glfwGetTime() * 0.5f) * glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram_03.ID, "transform"), 1, GL_FALSE, glm::value_ptr(rectangle_01_trans));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

	glDeleteBuffers(1, &rectangle_00_EBO);

	// GLFW: terminate, clearing all previously allocated resources
	glfwTerminate();
	return 0;
}

// process all input this frame (keys pressed, released, etc.)
void processInput(GLFWwindow* window)
{
	/*
	float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	*/

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixerValue += 0.001f;
		if (mixerValue > 1)
		{
			mixerValue = 1.0;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixerValue -= 0.001f;
		if (mixerValue < 0)
		{
			mixerValue = 0.0;
		}
	}
}

// GLFW: whenever the window size changes (OS or user resize) execute this
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);

	/*
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
	*/
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	/*
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
	*/
	camera.ProcessMouseScroll(yoffset);
}
