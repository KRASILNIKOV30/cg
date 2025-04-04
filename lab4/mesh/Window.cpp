#include "Window.h"

#include "DirectLight.h"
#include "Material.h"

#include <glm/gtx/orthonormalize.hpp>

namespace
{
// Угол обзора по вертикали
constexpr double FIELD_OF_VIEW = 60 * M_PI / 180.0;

constexpr double Z_NEAR = 0.1;
constexpr double Z_FAR = 10;

// Ортонормируем матрицу 4*4 (это должна быть аффинная матрица)
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

// Вращаем камеру вокруг начала координат
void Window::RotateCamera(double xAngleRadians, double yAngleRadians)
{
	// Извлекаем из 1 и 2 строки матрицы камеры направления осей вращения,
	// совпадающих с экранными осями X и Y.
	// Строго говоря, для этого надо извлекать столбцы их обратной матрицы камеры, но так как
	// матрица камеры ортонормированная, достаточно транспонировать её подматрицу 3*3
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

	// Вычисляем соотношение сторон клиентской области окна
	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	const auto proj = glm::perspective(FIELD_OF_VIEW, aspect, Z_NEAR, Z_FAR);
	glLoadMatrixd(&proj[0][0]);
	glMatrixMode(GL_MODELVIEW);
}

void Window::OnRunStart()
{
	// Сторона примитива считается лицевой, если при ее рисовании
	// обход верших осуществляется против часовой стрелки
	glFrontFace(GL_CCW);

	// Включаем тест глубины для удаления невидимых линий и поверхностей
	glEnable(GL_DEPTH_TEST);
	// Задаем ширину линий

	// И цвет очистки буфера цвета
	glClearColor(0, 0, 0, 1);
}

void Window::Draw(int width, int height)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetupCameraMatrix();
	SetupLightsAndMaterial();

	// glLineWidth(3);
	// m_frame.Draw();
	glPolygonMode(GL_FRONT, GL_FILL);
	glLineWidth(1);
	m_surface.Draw();
}

void Window::SetupCameraMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(&m_cameraMatrix[0][0]);
}

void Window::SetupLightsAndMaterial()
{
	// Включаем освещение и источник света №0
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Задаем параметры направленного источника света
	DirectLight light({ 2, 2, 2 });
	light.SetSpecularIntensity({ 1, 1, 1, 1 });
	light.SetDiffuseIntensity({ 1, 1, 1, 1 });

	// Задаем параметры материала
	Material material;
	material.SetDiffuse(0.8f, 0.8f, 0.f);
	material.SetSpecular(0.7f, 0.7f, 0.7f);
	material.SetShininess(50);
	material.Activate(GL_FRONT_AND_BACK);
}