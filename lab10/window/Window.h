#pragma once
#include "../../lib/glfwWindow/GLEWInitializer.h"
#include "../../lib/glfwWindow/BaseWindow.h"

#include "../vector/VectorMath.h"
#include "../Gauss.h"
#include "Planets.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <optional>

class Window final : public BaseWindow
{
public:
	Window(int w, int h, const char* title);

private:
	void OnMouseButton(int button, int action, [[maybe_unused]] int mods) override;
	void OnMouseMove(double x, double y) override;
	void RotateCamera(double xAngleRadians, double yAngleRadians);
	void OnResize(int width, int height) override;
	void OnRunStart() override;
	void InitShaders();
	void Draw(int width, int height) override;
	void SetupCameraMatrix();
	void OnKey(int /*key*/, int /*scanCode*/, int /*action*/, int /*mods*/) override;
	void OnIdle(double deltaTime) override;
	void DrawSphere(const Vector3f& pos, double radius, const Vector4f& color);
	void DrawPlanets(bool black);
	GLuint DrawGlowMap(int width, int height);
	void DrawScene(int width, int height);
	static void DrawRectangle(int width, int height);
	static void PushOrtho(int width, int height);
	static void PopOrtho();
	static void ApplyGlow(int width, int height, GLuint texture);
	void GenerateStars();
	void DrawStars();

private:
	std::vector<Planet> m_planets = planets;
	std::vector<Star> m_stars;
	double m_animationTime = 0.0;

	std::optional<GLEWInitializer> m_glewInitializer;
	static constexpr double DISTANCE_TO_ORIGIN = 15;
	bool m_leftButtonPressed = false;
	glm::dvec2 m_mousePos = {};
	glm::dmat4x4 m_cameraMatrix = glm::lookAt(
		glm::dvec3{ 0.0, 0.0, DISTANCE_TO_ORIGIN },
		glm::dvec3{ 0.0, 0.0, 0.0 },
		glm::dvec3{ 0.0, 1.0, 0.0 });
	GLUquadric* m_sphereQuadric;

	GLuint m_glowFBO;
	GLuint m_glowTexture;
	GLuint m_glowDepthBuffer;
	std::optional<Gauss> m_blur;

	int m_width = 800;
	int m_height = 600;
};