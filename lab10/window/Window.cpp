#include "Window.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/orthonormalize.hpp>

namespace
{
constexpr double FIELD_OF_VIEW = 60 * M_PI / 180.0;
constexpr double Z_NEAR = 0.1;
constexpr double Z_FAR = 100;
constexpr int DOWNSAMPLE_FACTOR = 2;

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

	const int glowWidth = width / DOWNSAMPLE_FACTOR;
	const int glowHeight = height / DOWNSAMPLE_FACTOR;

	glBindTexture(GL_TEXTURE_2D, m_glowTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glowWidth, glowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glBindRenderbuffer(GL_RENDERBUFFER, m_glowDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, glowWidth, glowHeight);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Window::OnRunStart()
{
	const int glowWidth = m_width / DOWNSAMPLE_FACTOR;
	const int glowHeight = m_height / DOWNSAMPLE_FACTOR;

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	m_sphereQuadric = gluNewQuadric();
	gluQuadricNormals(m_sphereQuadric, GLU_SMOOTH);

	m_glewInitializer.emplace();
	if (!__GLEW_ARB_shader_objects)
	{
		throw std::runtime_error("Shaders are not supported");
	}

	InitShaders();

	glGenFramebuffers(1, &m_glowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_glowFBO);

	glGenTextures(1, &m_glowTexture);
	glBindTexture(GL_TEXTURE_2D, m_glowTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glowWidth, glowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_glowTexture, 0);

	glGenRenderbuffers(1, &m_glowDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_glowDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, glowWidth, glowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_glowDepthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("Glow FBO not complete");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::InitShaders()
{
	m_blur.emplace();
}

GLuint Window::DrawGlowMap(int width, int height)
{
	const int glowWidth = width / DOWNSAMPLE_FACTOR;
	const int glowHeight = height / DOWNSAMPLE_FACTOR;

	glBindFramebuffer(GL_FRAMEBUFFER, m_glowFBO);
	glViewport(0, 0, glowWidth, glowHeight);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetupCameraMatrix();

	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	DrawSphere({ 0, 0, 0 }, 2.4f, { 1.0f, 1.0f, 0.0f, 1.0f });
	DrawPlanets(true);

	GLuint blurredTexture = m_blur->Blur(m_glowTexture, glowWidth, glowHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return blurredTexture;
}

void Window::DrawScene(int width, int height)
{
	SetupCameraMatrix();

	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	DrawSphere({ 0, 0, 0 }, 2.2f, { 1.0f, 1.0f, 0.0f, 1.0f });

	GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHTING);

	DrawPlanets(false);
}

void Window::PushOrtho(int width, int height)
{
	glDisable(GL_DEPTH_TEST); // Отключаем тест глубины, он больше не нужен
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void Window::PopOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Window::ApplyGlow(int width, int height, GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	DrawRectangle(width, height);

	glDisable(GL_TEXTURE_2D);
}

void Window::Draw(int width, int height)
{
	const auto blurredTexture = DrawGlowMap(width, height);
	DrawScene(width, height);

	PushOrtho(width, height);
	ApplyGlow(width, height, blurredTexture);
	PopOrtho();
}

void Window::DrawRectangle(int width, int height)
{
	const auto w = static_cast<float>(width);
	const auto h = static_cast<float>(height);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, h);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(w, h);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(w, 0.0f);
	}
	glEnd();
}

void Window::SetupCameraMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(&m_cameraMatrix[0][0]);
}

void Window::OnKey(int key, int /*scanCode*/, int action, int /*mods*/)
{
}

void Window::OnIdle(double deltaTime)
{
	m_animationTime += deltaTime;
}

void Window::DrawSphere(const Vector3f& pos, double radius, const Vector4f& color)
{
	glPushMatrix();
	glTranslated(pos.x, pos.y, pos.z);

	glColor4f(color.x, color.y, color.z, color.w);

	gluSphere(m_sphereQuadric, radius, 32, 32);

	glPopMatrix();
}

void Window::DrawPlanets(bool black)
{
	for (const auto& planet : m_planets)
	{
		glPushMatrix();

		const double angle = m_animationTime * planet.speed;
		glRotated(angle, 0.0, 1.0, 0.0);
		glTranslated(planet.orbitRadius, 0, 0);
		if (black)
		{
			glColor4f(0, 0, 0, 1);
		}
		else
		{
			glColor4f(planet.color.x, planet.color.y, planet.color.z, planet.color.w);
		}
		gluSphere(m_sphereQuadric, planet.radius, 32, 32);

		glPopMatrix();
	}
}
