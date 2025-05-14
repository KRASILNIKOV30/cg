#include "Window.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/orthonormalize.hpp>

namespace
{
constexpr double FIELD_OF_VIEW = 60 * M_PI / 180.0;
constexpr double Z_NEAR = 0.1;
constexpr double Z_FAR = 10;

glm::dmat4x4 Orthonormalize(const glm::dmat4x4& m)
{
	const auto normalizedMatrix = glm::orthonormalize(glm::dmat3x3{ m });
	return {
		glm::dvec4{ normalizedMatrix[0], 0.0 },
		glm::dvec4{ normalizedMatrix[1], 0.0 },
		glm::dvec4{ normalizedMatrix[2], 0.0 },
		m[3]
	};
}

}

Window::Window(int w, int h, const char* title)
	: BaseWindow(w, h, title)
{
}

void Window::OnMouseButton(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		m_leftButtonPressed = (action & GLFW_PRESS) != 0;
	}
}

void Window::OnMouseMove(double x, double y)
{
	const glm::dvec2 mousePos{ x, y };
	if (m_leftButtonPressed)
	{
		const auto windowSize = GetFramebufferSize();

		const auto mouseDelta = mousePos - m_mousePos;
		const double xAngle = mouseDelta.y * M_PI / windowSize.y;
		const double yAngle = mouseDelta.x * M_PI / windowSize.x;
		RotateCamera(xAngle, yAngle);
	}
	m_mousePos = mousePos;
}

void Window::RotateCamera(double xAngleRadians, double yAngleRadians)
{
	const glm::dvec3 xAxis{
		m_cameraMatrix[0][0], m_cameraMatrix[1][0], m_cameraMatrix[2][0]
	};
	const glm::dvec3 yAxis{
		m_cameraMatrix[0][1], m_cameraMatrix[1][1], m_cameraMatrix[2][1]
	};
	m_cameraMatrix = glm::rotate(m_cameraMatrix, xAngleRadians, xAxis);
	m_cameraMatrix = glm::rotate(m_cameraMatrix, yAngleRadians, yAxis);

	m_cameraMatrix = Orthonormalize(m_cameraMatrix);
}

void Window::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	const auto proj = glm::perspective(FIELD_OF_VIEW, aspect, Z_NEAR, Z_FAR);
	glLoadMatrixd(&proj[0][0]);
	glMatrixMode(GL_MODELVIEW);
}

void Window::OnRunStart()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	m_glewInitializer.emplace();
	if (!GLEW_ARB_shader_objects)
	{
		throw std::runtime_error("Shaders are not supported");
	}

	InitShaders();
}

void Window::InitShaders()
{
	m_canabola.emplace();
	m_flag.emplace();
}

void Window::Draw(int width, int height)
{
	glClearColor(0.8, 0.8, 0.8, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	SetupCameraMatrix();

	m_flag->Render();
	// m_canabola->Render();
}

void Window::SetupCameraMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(&m_cameraMatrix[0][0]);
}

void Window::OnKey(int key, int /*scanCode*/, int action, int /*mods*/)
{
	if (action == GLFW_PRESS && key == ' ')
	{
		m_usePointLight = !m_usePointLight;
		glfwPostEmptyEvent();
	}
	else if (action == GLFW_PRESS && key == 'S')
	{
		m_useSpecular = !m_useSpecular;
		glfwPostEmptyEvent();
	}
}