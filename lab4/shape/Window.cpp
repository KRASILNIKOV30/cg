#include "pch.h"
#include "Window.h"
#include "DirectLight.h"

namespace
{
// Угол обзора по вертикали
constexpr double FIELD_OF_VIEW = 60 * M_PI / 180.0;
// Размер стороны куба
constexpr double SHAPE_SIZE = 1;

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
	  , m_dodecahedron(SHAPE_SIZE)
{
#if 1
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_0, { 0.95f, 0.20f, 0.20f, 1.0f });
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_1, { 0.07f, 0.62f, 0.92f, 1.0f });
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_2, { 1.00f, 0.84f, 0.00f, 1.0f });
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_3, { 0.60f, 0.20f, 0.80f, 1.0f });
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_4, { 0.20f, 0.80f, 0.20f, 1.0f });
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_5, { 1.00f, 0.50f, 0.00f, 1.0f });
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_6, { 0.00f, 0.70f, 0.70f, 1.0f });
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_7, { 0.90f, 0.10f, 0.50f, 1.0f });
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_8, { 0.40f, 0.40f, 0.90f, 1.0f });
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_9, { 0.80f, 0.80f, 0.20f, 1.0f });
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_10, { 0.90f, 0.60f, 0.70f, 1.0f });
	m_dodecahedron.SetFaceColor(DodecahedronFace::FACE_11, { 0.30f, 0.90f, 0.60f, 1.0f });
#endif

	m_dodecahedron.SetSpecularColor({ 1, 1, 1, 1 });
	m_dodecahedron.SetShininess(4.0f);
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
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Включаем режим отбраковки граней
	glEnable(GL_CULL_FACE);
	// Отбраковываться будут нелицевые стороны граней
	glCullFace(GL_BACK);
	// Сторона примитива считается лицевой, если при ее рисовании
	// обход верших осуществляется против часовой стрелки
	glFrontFace(GL_CCW);

	// Включаем тест глубины для удаления невидимых линий и поверхностей
	glEnable(GL_DEPTH_TEST);

	// Направление на источник света (совпадает с позицией наблюдателя)
	DirectLight light{ { 0.0f, 0.0f, 1.0f } };
	light.SetDiffuseIntensity({ 0.4f, 0.4f, 0.4f, 1.0f });
	light.SetAmbientIntensity({ 0.1f, 0.1f, 0.1f, 1.0f });
	light.SetSpecularIntensity({ 0.3f, 0.3f, 0.3f, 1.0f });
	light.Apply(GL_LIGHT0);
}

void Window::Draw(int width, int height)
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetupCameraMatrix();

	m_dodecahedron.Draw();
}

void Window::SetupCameraMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(&m_cameraMatrix[0][0]);
}
