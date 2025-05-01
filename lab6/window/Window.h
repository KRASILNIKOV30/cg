#pragma once
#include "../../lib/glfwWindow/BaseWindow.h"
#include "lab6/3d/model/Model.h"
#include "lab6/3d/model/ModelRenderer.h"

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
	Model m_model;
	ModelRenderer m_renderer;
	glm::dmat4x4 m_cameraMatrix = glm::lookAt(
		glm::dvec3{ 1.7, 1.7, 1.7 },
		glm::dvec3{ 0.0, 0.0, 0.0 },
		glm::dvec3{ 0.0, 0.0, 1.0 });
};