#include<iostream>
#include "camera.hpp"
#include "shader.hpp"
#include "hey.hpp"

Camera cam;

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
        glfwTerminate();
        return -1;
    }
	Shader shader("vertex.vs", "fragment.fs");
    glViewport(0, 0, 640, 480);
    float* vertices = new float[N_VERTICES];
    unsigned int *indices = new unsigned int[N_INDICES];
    meshgrid(vertices, indices);


    unsigned int EBO;
    unsigned int VBO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, N_INDICES * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, N_VERTICES * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    float lt = glfwGetTime();
    while (glfwWindowShouldClose(window) == false)
    {
        float t = glfwGetTime();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = cam.view();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)640/ (float)480, 0.1f, 100.0f);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
        cam.processInput(window, t - lt);
        shader.setUm4f("model", glm::value_ptr(model), GL_FALSE);
        shader.setUm4f("view", glm::value_ptr(view), GL_FALSE);
        shader.setUm4f("projection", glm::value_ptr(projection), GL_FALSE);
        shader.use();
        t = lt;
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, N_VERTICES * sizeof(float), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, N_INDICES * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, N_INDICES, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwWaitEventsTimeout(.03);
    }


    glfwTerminate();
    delete[] indices;
    delete[] vertices;
    return 0;
}