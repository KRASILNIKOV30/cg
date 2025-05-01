#pragma once
#include "../../lab3/tetris/model/Model.h"
#include "../lib/BaseWindow.h"
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

class Window final : public BaseWindow
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
	Model m_model;
	glm::dmat4x4 m_cameraMatrix = glm::lookAt(
		glm::dvec3{ 1.7, 1.7, 1.7 },
		glm::dvec3{ 0.0, 0.0, 0.0 },
		glm::dvec3{ 0.0, 0.0, 1.0 });
};