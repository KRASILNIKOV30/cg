#include "./Window.h"

#include <GL/glu.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/log_base.hpp>
#include <glm/gtx/orthonormalize.hpp>

namespace
{
// Угол обзора по вертикали
constexpr double FIELD_OF_VIEW = 60 * M_PI / 180.0;

constexpr double Z_NEAR = 0.1;
constexpr double Z_FAR = 20;

constexpr float MOVE_SPEED = 0.05f;
constexpr float ROT_SPEED = 0.03f;

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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);

	glClearColor(0, 0, 0, 1);
}

void Window::Draw(int width, int height)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetupCamera();
	ProcessInput();

	m_maze.Draw();
}

void Window::ProcessInput()
{
	if (IsKeyPressed(GLFW_KEY_LEFT))
	{
		m_playerYaw += ROT_SPEED;
	}
	if (IsKeyPressed(GLFW_KEY_RIGHT))
	{
		m_playerYaw -= ROT_SPEED;
	}

	float moveX = 0.0f;
	float moveZ = 0.0f;

	if (IsKeyPressed(GLFW_KEY_W))
	{
		moveX = sinf(m_playerYaw) * MOVE_SPEED;
		moveZ = cosf(m_playerYaw) * MOVE_SPEED;
	}
	if (IsKeyPressed(GLFW_KEY_S))
	{
		moveX = -sinf(m_playerYaw) * MOVE_SPEED;
		moveZ = -cosf(m_playerYaw) * MOVE_SPEED;
	}
	if (IsKeyPressed(GLFW_KEY_A))
	{
		moveX = cosf(m_playerYaw) * MOVE_SPEED;
		moveZ = -sinf(m_playerYaw) * MOVE_SPEED;
	}
	if (IsKeyPressed(GLFW_KEY_D))
	{
		moveX = -cosf(m_playerYaw) * MOVE_SPEED;
		moveZ = sinf(m_playerYaw) * MOVE_SPEED;
	}

	constexpr float delta = 1.5 * Z_NEAR;
	if (m_maze.CanMoveTo(m_playerPos.x + moveX + delta * glm::sign(moveX), m_playerPos.z))
	{
		m_playerPos.x += moveX;
	}
	if (m_maze.CanMoveTo(m_playerPos.x, m_playerPos.z + moveZ + delta * glm::sign(moveZ)))
	{
		m_playerPos.z += moveZ;
	}
}

void Window::SetupCamera() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const auto screenSize = GetFramebufferSize();
	gluPerspective(60.0, static_cast<float>(screenSize[0]) / static_cast<float>(screenSize[1]), Z_NEAR, Z_FAR);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	const auto lookX = m_playerPos.x + sinf(m_playerYaw);
	const auto lookZ = m_playerPos.z + cosf(m_playerYaw);

	gluLookAt(m_playerPos.x, m_playerPos.y, m_playerPos.z,
		lookX, m_playerPos.y, lookZ,
		0.0f, 1.0f, 0.0f);
}
