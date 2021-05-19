#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
private:
	glm::vec3 position  = glm::vec3(-2.0f, 2.0f, 4.0f);
	glm::vec3 direction = glm::vec3(.0f, .0f, -1.f);
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp = glm::vec3(0.0f, 1.f, 0.f);
	float pitch, yaw;
	float radius = 10.0f;
	float speed = .0133f, mouse_sensitivity = .05f;
	float lastCursurX = 320.f,
		lastCursurY = 240.f;
	bool first_mouse;
	void update()
	{
		if (yaw < -180.f) yaw += 360.f;
		if (yaw > 180.f) yaw -= 360.f;
		if (pitch < -90.f) pitch += 180.f;
		if (pitch > 90.f) pitch -= 180.f;
		glm::vec3 front(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
			sin(glm::radians(pitch)),
			sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

		direction = glm::normalize(front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

public:

	Camera() {
		up = worldUp;
		yaw = -60.0f;
		pitch = -20.0f;
		update();
	}

	glm::mat4 view()
	{
		return glm::lookAt(position, position + direction, up);
	}

	void processInput(GLFWwindow* window, float dt)
	{
		bool upd = false;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			position += speed * direction * dt; upd = true;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			position -= speed * direction * dt; upd = true;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			position -= speed * right * dt; upd = true;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			position += speed * right * dt; upd = true;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			yaw -= 10 * speed * dt;
			upd = true;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			yaw += 10 * speed * dt;
			upd = true;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			pitch += 10 * speed * dt;
			upd = true;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			pitch -= 10 * speed * dt;
			upd = true;
		}
		if (upd)
			update();
	}

	const glm::vec3& getPosition() const {
		return position;
	}

};


void mouse_callback(GLFWwindow* window, double x, double y);


#endif