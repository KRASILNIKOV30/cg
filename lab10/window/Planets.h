#pragma once
#include "../vector/Vector4.h"
#include <vector>

struct Planet
{
	float radius;
	float orbitRadius;
	float speed;
	Vector4f color;
};

inline const std::vector<Planet> planets = {
	{ 0.2, 3, 50, { 0.8, 0.2, 0.1, 1 } },
	{ 0.4, 5, 30, { 0.2, 0.8, 0.3, 1 } },
	{ 1, 7, 20, { 0.9, 0.7, 0.2, 1 } }
};

struct Star
{
	Vector3f pos;
	float size;
};