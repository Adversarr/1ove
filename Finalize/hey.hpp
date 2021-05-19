#pragma once

#include <cmath>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>


#define MESH_WIDTH 30
#define MESH_HEIGHT 30
#define MESH_R 0.5f
#define N_VERTICES (MESH_HEIGHT * MESH_WIDTH * 3)
#define N_INDICES ((MESH_HEIGHT-1) * MESH_WIDTH * 6)

constexpr float pi = glm::pi<float>();


inline float func(float x, float y, float z)
{
	float x2 = x * x, y2 = 2.25f * y * y, z2 = z * z;
	float t = (x2 + y2 + z2 - 1);
	t = t * t * t;
	return t - (x2 + 0.025f * y2) * z2 * z;
}

void meshgrid(float* position, unsigned int* index)
{
	// generate 10 * 10 meshgrid:
	for (int i = 0; i < MESH_HEIGHT; ++i) {
		for (int j = 0; j < MESH_WIDTH; ++j) {
			float theta = ((float)i) / ((float)MESH_HEIGHT - 1.f) * 2 * pi;
			float phi = ((float)j) / ((float)MESH_WIDTH - 1.f) * pi;
			position[3 * (MESH_WIDTH * i + j)] = MESH_R * sin(phi) * cos(theta);
			position[3 * (MESH_WIDTH * i + j) + 1] = MESH_R * cos(phi);
			position[3 * (MESH_WIDTH * i + j) + 2] = MESH_R * sin(phi) * sin(theta);
		}
	}
	// generate indexes:
	int n = 0;
	for (int i = 0; i < MESH_HEIGHT - 1; ++i) {
		for (int j = 1; j < MESH_WIDTH; ++j) {
			index[n++] = i * MESH_WIDTH + j - 1;
			index[n++] = i * MESH_WIDTH + j;
			index[n++] = i * MESH_WIDTH + j + MESH_WIDTH - 1;
			index[n++] = i * MESH_WIDTH + j;
			index[n++] = i * MESH_WIDTH + j + MESH_WIDTH - 1;
			index[n++] = i * MESH_WIDTH + j + MESH_WIDTH;
		}
		index[n++] = i * MESH_WIDTH + MESH_WIDTH - 1;
		index[n++] = i * MESH_WIDTH;
		index[n++] = (i + 1) * MESH_WIDTH;
		index[n++] = (i + 1) * MESH_WIDTH - 1;
		index[n++] = (i + 1) * MESH_WIDTH;
		index[n++] = (i + 3) * MESH_WIDTH - 1;
	}
}
