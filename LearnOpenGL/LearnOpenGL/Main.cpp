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

// Screen.
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera.
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));	// Camera zoom

// Mouse.
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing.
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Lighting.
glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

// Material.
float mixerValue = 0.0;

float generate_normalized_random_float()
{
	return (float)(rand()) / (float)(RAND_MAX);
}

int main() {
	// GLFW: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLFW: capture mouse input.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Build and compile shader program(s).
	Shader cubeColorShaderProgram("cube_color.vs", "cube_color.fs");
	Shader lightCubeColorShaderProgram("light_color.vs", "light_color.fs");

	float cube_00_vertices[] = {
			// positions (x, y, z)	// normals (x, y, z)	// texture coords (u, v)
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f
	};

	unsigned int cube_00_indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// ========== BEGIN GENERIC CUBE ==========
	unsigned int cube_00_VAO;
	unsigned int cube_00_VBO;
	unsigned int cube_00_EBO;

	glGenVertexArrays(1, &cube_00_VAO);
	glGenBuffers(1, &cube_00_VBO);
	glGenBuffers(1, &cube_00_EBO);

	glBindVertexArray(cube_00_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, cube_00_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_00_EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_00_vertices), cube_00_vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_00_indices), cube_00_indices, GL_STATIC_DRAW);

	// Position vertex attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal vertex attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture coordinate vertex attribute (UVs).
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// ========== END GENERIC CUBE ==========

	// ========== BEGIN LIGHT CUBE ==========
	unsigned int light_cube_00_VAO;
	unsigned int light_cube_00_VBO;
	unsigned int light_cube_00_EBO;

	glGenVertexArrays(1, &light_cube_00_VAO);
	glGenBuffers(1, &light_cube_00_VBO);
	glGenBuffers(1, &light_cube_00_EBO);

	glBindVertexArray(light_cube_00_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, light_cube_00_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_cube_00_EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_00_vertices), cube_00_vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_00_indices), cube_00_indices, GL_STATIC_DRAW);

	// Position vertex attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ========== END LIGHT CUBE ==========

	// ========== BEGIN TEXTURE SETUP ==========
	unsigned int texture_00, texture_01;
	glGenTextures(1, &texture_00);
	glGenTextures(1, &texture_01);

	// Setup texture 0.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_00);

	// Set the texture wrapping/filtering options (on the currently bound texture object).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load and generate the texture.
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Resources/Textures/awesomeface.png", &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true);
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

	// Setup texture 1.
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_01);

	// Set the texture wrapping/filtering options (on the currently bound texture object).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load and generate the texture.
	data = stbi_load("Resources/Textures/judge_judy.jpg", &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(false);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// ========== END TEXTURE SETUP ==========

	// Draw in wireframe mode.
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

	glm::vec3 cubeColors[] = {
	glm::vec3(generate_normalized_random_float(), generate_normalized_random_float(), generate_normalized_random_float()),
	glm::vec3(generate_normalized_random_float(), generate_normalized_random_float(), generate_normalized_random_float()),
	glm::vec3(generate_normalized_random_float(), generate_normalized_random_float(), generate_normalized_random_float()),
	glm::vec3(generate_normalized_random_float(), generate_normalized_random_float(), generate_normalized_random_float()),
	glm::vec3(generate_normalized_random_float(), generate_normalized_random_float(), generate_normalized_random_float()),
	glm::vec3(generate_normalized_random_float(), generate_normalized_random_float(), generate_normalized_random_float()),
	glm::vec3(generate_normalized_random_float(), generate_normalized_random_float(), generate_normalized_random_float()),
	glm::vec3(generate_normalized_random_float(), generate_normalized_random_float(), generate_normalized_random_float()),
	glm::vec3(generate_normalized_random_float(), generate_normalized_random_float(), generate_normalized_random_float()),
	glm::vec3(generate_normalized_random_float(), generate_normalized_random_float(), generate_normalized_random_float())
	};

	struct Material 
	{
		std::string name;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
	};

	// http://devernay.free.fr/cours/opengl/materials.html
	Material cubeMaterials[24] = 
	{
		{"emerald", glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f), glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f},
		{"jade", glm::vec3(0.135f, 0.2225f, 0.1575f), glm::vec3(0.54f, 0.89f, 0.63f), glm::vec3(0.316228f, 0.316228f, 0.316228f), 0.1f},
		{"obsidian", glm::vec3(0.05375f, 0.05f, 0.06625f), glm::vec3(0.18275f, 0.17f, 0.22525f), glm::vec3(0.332741f, 0.328634f, 0.346435f), 0.3f},
		{"pearl", glm::vec3(0.25f, 0.20725f, 0.20725f), glm::vec3(1.0f, 0.829f, 0.829f), glm::vec3(0.296648f, 0.296648f, 0.296648f), 0.088f},
		{"ruby", glm::vec3(0.1745f, 0.01175f, 0.01175f), glm::vec3(0.61424f, 0.04136f, 0.04136f), glm::vec3(0.727811f, 0.626959f, 0.626959f), 0.6f},
		{"turquoise", glm::vec3(0.1f, 0.18725f, 0.1745f), glm::vec3(0.396f, 0.74151f, 0.69102f), glm::vec3(0.297254f, 0.30829f, 0.306678f), 0.1f},
		{"brass", glm::vec3(0.329412f, 0.223529f, 0.027451f), glm::vec3(0.780392f, 0.568627f, 0.113725f), glm::vec3(0.992157f, 0.941176f, 0.807843f), 0.21794872f},
		{"bronze", glm::vec3(0.2125f, 0.1275f, 0.054f), glm::vec3(0.714f, 0.4284f, 0.18144f), glm::vec3(0.393548f, 0.271906f, 0.166721f), 0.2f},
		{"chrome", glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6f},
		{"copper", glm::vec3(0.19125f, 0.0735f, 0.0225f), glm::vec3(0.7038f, 0.27048f, 0.0828f), glm::vec3(0.256777f, 0.137622f, 0.086014f), 0.1f},
		{"gold", glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f), glm::vec3(0.628281f, 0.555802f, 0.366065f), 0.4f},
		{"silver", glm::vec3(0.19225f, 0.19225f, 0.19225f), glm::vec3(0.50754f, 0.50754f, 0.50754f), glm::vec3(0.508273f, 0.508273f, 0.508273f), 0.4f},
		{"black plastic", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.50f, 0.50f, 0.50f), .25f},
		{"cyan plastic", glm::vec3(0.0f, 0.1f, 0.06f), glm::vec3(0.0f, 0.50980392f, 0.50980392f), glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), .25f},
		{"green plastic", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.35f, 0.1f), glm::vec3(0.45f, 0.55f, 0.45f), .25f},
		{"red plastic", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.7f, 0.6f, 0.6f), .25f},
		{"white plastic", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.55f, 0.55f, 0.55f), glm::vec3(0.70f, 0.70f, 0.70f), .25f},
		{"yellow plastic", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.60f, 0.60f, 0.50f), .25f},
		{"black rubber", glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.4f, 0.4f, 0.4f), .078125f},
		{"cyan rubber", glm::vec3(0.0f, 0.05f, 0.05f), glm::vec3(0.4f, 0.5f, 0.5f), glm::vec3(0.04f, 0.7f, 0.7f), .078125f},
		{"green rubber", glm::vec3(0.0f, 0.05f, 0.0f), glm::vec3(0.4f, 0.5f, 0.4f), glm::vec3(0.04f, 0.7f, 0.04f), .078125f},
		{"red rubber", glm::vec3(0.05f, 0.0f, 0.0f), glm::vec3(0.5f, 0.4f, 0.4f), glm::vec3(0.7f, 0.04f, 0.04f), .078125f},
		{"white rubber", glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), .078125f},
		{"yellow rubber", glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(0.5f, 0.5f, 0.4f), glm::vec3(0.7f, 0.7f, 0.04f), .078125f}
	};


	// ========== BEGIN RENDER LOOP ==========
	while (!glfwWindowShouldClose(window))
	{
		// Timing.
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input.
		processInput(window);

		// General render operations.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		
		// Move (rotate) light over time.
		lightPosition.x = sin(glfwGetTime() * 0.35f) * 35.0f;
		lightPosition.y = 0;
		lightPosition.z = cos(glfwGetTime() * 0.35f) * 35.0f;

		// Change color of light over time.
		//lightColor.x = sin(glfwGetTime() * 1.0f);
		//lightColor.y = sin(glfwGetTime() * 0.35f);
		//lightColor.z = sin(glfwGetTime() * 0.65f);

		// ========== BEGIN RENDER GENERIC CUBES ==========
		// Always activate the shader before setting the uniforms.  
		cubeColorShaderProgram.use();

		glUniform1i(glGetUniformLocation(cubeColorShaderProgram.ID, "texture_00"), 0.0f); // Set the texture uniform manually.
		//cubeColorShaderProgram.setInt("texture_0", 1); // Set the texture uniform using the shader class.
		glUniformMatrix4fv(glGetUniformLocation(cubeColorShaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(cubeColorShaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(cubeColorShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Default (static) cube parameters.
		cubeColorShaderProgram.setFloat("textureCoordinateScale", 1.0f);
		cubeColorShaderProgram.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		cubeColorShaderProgram.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		cubeColorShaderProgram.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		cubeColorShaderProgram.setFloat("material.shininess", 256.0f);
		//cubeColorShaderProgram.setVec3("light.ambientStrength", 1.0f, 0.2f, 0.2f);
		//cubeColorShaderProgram.setVec3("light.diffuseStrength", 0.5f, 0.5f, 0.5f);
		//cubeColorShaderProgram.setVec3("light.specularStrength", 1.0f, 1.0f, 1.0f);
		cubeColorShaderProgram.setVec3("light.ambientStrength", 1.0f, 1.0f, 1.0f);
		cubeColorShaderProgram.setVec3("light.diffuseStrength", 1.0f, 1.0f, 1.0f);
		cubeColorShaderProgram.setVec3("light.specularStrength", 1.0f, 1.0f, 1.0f);

		// Variable (dynamic) cube parameters.
		cubeColorShaderProgram.setFloat("mixerValue", mixerValue);
		cubeColorShaderProgram.setVec3("viewPosition", camera.Position);
		cubeColorShaderProgram.setVec3("lightPosition", lightPosition);
		cubeColorShaderProgram.setVec3("lightColor", lightColor);

		// Position, normals and UVs.
		glBindVertexArray(cube_00_VAO);

		// Generate random cubes within the scene.
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			float rotationSpeed = 0.25f;
			if (i == 0 || i % 2 == 0) 
			{
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle) * rotationSpeed, glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else 
			{
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			glUniformMatrix4fv(glGetUniformLocation(cubeColorShaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

			// Variable (dynamic) cube parameters.
			cubeColorShaderProgram.setVec3("material.ambient", cubeMaterials[i].ambient);
			cubeColorShaderProgram.setVec3("material.diffuse", cubeMaterials[i].diffuse);
			cubeColorShaderProgram.setVec3("material.specular", cubeMaterials[i].specular);
			cubeColorShaderProgram.setFloat("material.shininess", cubeMaterials[i].shininess * 256.0f);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// ========== END RENDER GENERIC CUBES ==========

				// ========== BEGIN RENDER LIGHT CUBE ==========
		lightCubeColorShaderProgram.use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(lightPosition.x, lightPosition.y, lightPosition.z));

		glUniformMatrix4fv(glGetUniformLocation(lightCubeColorShaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(lightCubeColorShaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightCubeColorShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Variable (dynamic) light parameters.
		lightCubeColorShaderProgram.setVec3("lightColor", lightColor);

		glBindVertexArray(light_cube_00_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// ========== END RENDER LIGHT CUBE ==========
		
		// GLFW: Swap buffers and poll IO events.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources.
	glDeleteVertexArrays(1, &cube_00_VAO);
	glDeleteBuffers(1, &cube_00_VBO);
	glDeleteBuffers(1, &cube_00_EBO);

	glDeleteVertexArrays(1, &light_cube_00_VAO);
	glDeleteBuffers(1, &light_cube_00_VBO);
	glDeleteBuffers(1, &light_cube_00_EBO);

	// GLFW: clear all previously allocated resources.
	glfwTerminate();
	return 0;

	// ========== END RENDER LOOP ==========
}

// Process input each frame (keys pressed, released, etc.).
void processInput(GLFWwindow* window)
{
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

// GLFW: whenever the window size changes.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// GLFW: whenever the mouse moves.
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
}

// GLFW: whenever the mouse scrolls.
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}