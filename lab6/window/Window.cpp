#include "GL/glew.h"
#include "./Window.h"
#include "../3d/modelLoader/ModelLoader.h"
#include <GL/glu.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/log_base.hpp>
#include <glm/gtx/orthonormalize.hpp>

namespace
{
constexpr double FIELD_OF_VIEW = 60 * M_PI / 180.0;

constexpr double Z_NEAR = 0.1;
constexpr double Z_FAR = 20;

glm::dmat4x4 Orthonormalize(const glm::dmat4x4& m)
{
	// Извлекаем подматрицу 3*3 из матрицы m и ортонормируем её
	const auto normalizedMatrix = glm::orthonormalize(glm::dmat3x3{ m });
	// Заменяем 3 столбца исходной матрицы
	return {
		glm::dvec4{ normalizedMatrix[0], 0.0 },
		glm::dvec4{ normalizedMatrix[1], 0.0 },
		glm::dvec4{ normalizedMatrix[2], 0.0 },
		m[3]
	};
}

} // namespace

Window::Window(int w, int h, const char* title)
	: BaseWindow(w, h, title)
{
}

void Window::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	const auto aspect = static_cast<double>(width) / static_cast<double>(height);

	glMatrixMode(GL_PROJECTION);
	const auto proj = glm::perspective(FIELD_OF_VIEW, aspect, Z_NEAR, Z_FAR);
	glLoadMatrixd(&proj[0][0]);
	glMatrixMode(GL_MODELVIEW);
}

void Window::OnRunStart()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);

	const ModelLoader loader;
	loader.LoadObjFile("res/model.obj", m_model);

	auto const& modelBB = m_model.GetBoundingBox();

	if (modelBB.IsEmpty())
	{
		throw std::runtime_error("Model is empty");
	}

	auto const& modelCenter = modelBB.GetCenter();
	const auto modelDiagonal = modelBB.GetSize().GetLength();
	const auto cameraPosition = modelCenter + Vector3f(modelDiagonal, modelDiagonal / 2, modelDiagonal);
	glLoadIdentity();
	gluLookAt(
		cameraPosition.x, cameraPosition.y, cameraPosition.z,
		modelCenter.x, modelCenter.y, modelCenter.z,
		0, 1, 0);

}

void Window::Draw(int width, int height)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	SetupCamera();
	ProcessInput();

	// glColor3f(0.0, 0.0, 1.0);

	ModelRenderer::RenderModel(m_model);
}

void Window::ProcessInput()
{

}

void Window::SetupCamera() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const auto screenSize = GetFramebufferSize();
	gluPerspective(60.0, static_cast<float>(screenSize[0]) / static_cast<float>(screenSize[1]), Z_NEAR, Z_FAR);
}
