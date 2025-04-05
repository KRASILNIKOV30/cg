#pragma once
#include "Maze.h"
#include "../lib/BaseWindow.h"
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

class Window : public BaseWindow
{
public:
	Window(int w, int h, const char* title);

private:
	void OnResize(int width, int height) override;
	void OnRunStart() override;
	void Draw(int width, int height) override;

	void ProcessInput();
	void SetupCamera() const;

private:
	Maze m_maze;
	glm::vec3 m_playerPos = { 1.5f, 0.5f, 1.5f };
	float m_playerYaw = 0.0f;
	glm::dmat4x4 m_cameraMatrix = glm::lookAt(
		glm::dvec3{ 1.7, 1.7, 1.7 },
		glm::dvec3{ 0.0, 0.0, 0.0 },
		glm::dvec3{ 0.0, 0.0, 1.0 });
};