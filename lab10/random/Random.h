#pragma once
#include <random>

inline double RandomDouble()
{
	thread_local std::mt19937 generator(std::random_device{}());
	thread_local std::uniform_real_distribution distribution(0.0, 1.0);
	return distribution(generator);
}

inline float RandomFloat(float min, float max)
{
	return static_cast<float>(std::lerp(min, max, RandomDouble()));
}