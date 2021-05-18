#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>


const char* vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"out vec4 vertexColor;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection*view*model*vec4(aPos, 1.0f);\n"
	"	vertexColor = vec4(aPos, 1.0);\n"
	"}\0";
const char* fragmentShaderSource = 
	"#version 330 core\n"
	"in vec4 vertexColor;"
	"uniform vec4 ourColor;\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(vertexColor.x, vertexColor.y, ourColor.z / 2 + .5f,1.0f);\n"
	"}\0";

void meshgrid(float* position, unsigned int* index)
{
	// generate 10 * 10 meshgrid:
	for (int i = 0; i < 30; ++i) {
		for (int j = 0; j < 30; ++j) {
			position[3 * (30 * i + j)] = ((float)i - 14.5f) / 30.0f;
			position[3 * (30 * i + j) + 1] = ((float)j - 14.5f) / 30.0f;
			position[3 * (30 * i + j) + 2] = 0.f;
		}
	}
	// generate indexes:
	int n = 0;
	for (int i = 0; i < 29; ++i) {
		for (int j = 1; j < 30; ++j) {
			index[n++] = i * 30 + j - 1;
			index[n++] = i * 30 + j;
			index[n++] = i * 30 + j + 29;
			index[n++] = i * 30 + j;
			index[n++] = i * 30 + j + 29;
			index[n++] = i * 30 + j + 30;
		}
	}
}


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(640, 480, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 640, 480);
	//float vertices[] = {
	//	0.5f, 0.5f, 0.0f,   // 右上角
	//	0.5f, -0.5f, 0.0f,  // 右下角
	//	-0.5f, -0.5f, 0.0f, // 左下角
	//	-0.5f, 0.5f, 0.0f   // 左上角
	//};
	/*unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};*/

	float* vertices = (float *) malloc(3 * 30 * 30 * sizeof(float));
	unsigned int* indices = (unsigned int*)malloc(6 * 29 * 29 * sizeof(unsigned int));
	meshgrid(vertices, indices);
	unsigned int vertexShader;
	

	unsigned int EBO;
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::cout << "Vertex.";
		glfwTerminate();
		return -1;
	}
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::cout << "Fragment.";
		glfwTerminate();
		return -1;
	}
	glCompileShader(fragmentShader);
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		std::cout << "Shader.";
		glfwTerminate();
		return -1;
	}

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 30 * 30 * sizeof(unsigned int), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 29 * 29 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	

	


	while (!glfwWindowShouldClose(window)) {
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		float timeValue = glfwGetTime();
		float blueValue = (sin(timeValue) / 2.f) + .5f;
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, - 3.f -1.3f * blueValue));
		projection = glm::perspective(glm::radians(45.0f), (float)640/ (float)480, 0.1f, 100.0f);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		glClearColor(.2f, .3f, .3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		unsigned int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model"),
			viewLoc = glGetUniformLocation(shaderProgram, "view"),
			projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		glUniform4f(vertexColorLocation, 0.f, 0.f, blueValue, 0.f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6 * 29 * 29, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glfwTerminate();
	return 0;
}