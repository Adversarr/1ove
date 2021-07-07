#pragma once

#include <cmath>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>


#define MESH_WIDTH 520
#define MESH_HEIGHT 520
#define MESH_R 2.0f
#define N_VERTICES (MESH_HEIGHT * MESH_WIDTH * 6)
#define N_INDICES ((MESH_HEIGHT-1) * MESH_WIDTH * 6)
#define RATE .999f
constexpr float pi = glm::pi<float>();


inline float func(float x, float y, float z)
{
// 	float x2 = x * x, y2 = 2.25f * y * y, z2 = z * z;
// 	float t = (x2 + y2 + z2 - 1);
// 	t = t * t * t;
// 	return t - (x2 + 0.025f * y2) * z2 * z;
	return x * x + y * y + z * z - 1;
}

inline float gradx(float x, float y, float z)
{
	float x2 = x * x, y2 = 2.25f * y * y, z2 = z * z;
	float t = (x2 + y2 + z2 - 1);
	return t * t * 2 * x - 2 * x * z2 * z;
}


inline float grady(float x, float y, float z)
{
	float x2 = x * x, y2 = y * y, z2 = z * z;
	float t = (x2 + y2 + z2 - 1);
	return 4.5f * y * t * t - .225f * y * z2 * z;
}


inline float gradz(float x, float y, float z)
{
	float x2 = x * x, y2 = 2.25f * y * y, z2 = z * z;
	float t = (x2 + y2 + z2 - 1);
	return 2 * z * t * t - 3 * z2 * x2 - 27.0f / 80.00f * y2 * z2;
}





void meshgrid(float* position, unsigned int* index)
{
	// generate 10 * 10 meshgrid:
	for (int i = 0; i < MESH_HEIGHT; ++i) {
		for (int j = 0; j < MESH_WIDTH; ++j) {
			float theta = ((float)i) / ((float)MESH_HEIGHT - 1.f) * 2 * pi;
			float phi = ((float)j) / ((float)MESH_WIDTH - 1.f) * pi;
			position[6 * (MESH_WIDTH * i + j)] = MESH_R * sin(phi) * cos(theta);
			position[6 * (MESH_WIDTH * i + j) + 1] = MESH_R * cos(phi);
			position[6 * (MESH_WIDTH * i + j) + 2] = MESH_R * sin(phi) * sin(theta);
		}
	}
	// generate indexes:
	int n = 0;
	for (int i = 0; i < MESH_HEIGHT - 1; ++i) {
		for (int j = 1; j < MESH_WIDTH; ++j) {
			index[n++] = i * MESH_WIDTH + j;
			index[n++] = i * MESH_WIDTH + j - 1;
			index[n++] = i * MESH_WIDTH + j + MESH_WIDTH - 1;
			index[n++] = i * MESH_WIDTH + j;
			index[n++] = i * MESH_WIDTH + j + MESH_WIDTH - 1;
			index[n++] = i * MESH_WIDTH + j + MESH_WIDTH;
		}
		index[n++] = i * MESH_WIDTH;
		index[n++] = i * MESH_WIDTH + MESH_WIDTH - 1;
		index[n++] = (i + 2) * MESH_WIDTH - 1;
		index[n++] = i * MESH_WIDTH;
		index[n++] = (i + 2) * MESH_WIDTH - 1;
		index[n++] = (i + 2) * MESH_WIDTH;
	}
}

bool shrink(float* pos) {
	float x = pos[0],
		y = pos[1],
		z = pos[2];
	float x_ = x * RATE,
		y_ = y * RATE,
		z_ = z * RATE;
	bool ret = false;
	if (func(x, z, y) * func(x_, z_, y_) > 0.0f)
	{
		pos[0] = x_;
		pos[1] = y_;
		pos[2] = z_;
		ret = true;
	}

	pos[3] = gradx(x, y, z);
	pos[4] = grady(x, y, z);
	pos[5] = gradz(x, y, z);

	glm::vec3 grad = glm::vec3(pos[3], pos[4], pos[5]);
	grad = glm::normalize(grad);
	pos[3] = grad[0];
	pos[4] = grad[1];
	pos[5] = grad[2];

	return ret;
}