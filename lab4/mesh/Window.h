#pragma once
#include "BaseWindow.h"
#include "Frame.h"
#include "SincSurface.h"
#include "Surface.h"

class Window : public BaseWindow
{
public:
	Window(int w, int h, const char* title);

private:
	void OnMouseButton(int button, int action, [[maybe_unused]] int mods) override;

	void OnMouseMove(double x, double y) override;

	// Âðàùàåì êàìåðó âîêðóã íà÷àëà êîîðäèíàò
	void RotateCamera(double xAngleRadians, double yAngleRadians);

	void OnResize(int width, int height) override;

	void OnRunStart() override;

	void Draw(int width, int height) override;

	void SetupCameraMatrix();

	Frame m_frame{ 5 };
	SincSurface m_surface = { 100, 100, -10, 10, -10, 10 };

	bool m_leftButtonPressed = false;
	glm::dvec2 m_mousePos = {};
	glm::dmat4x4 m_cameraMatrix = glm::lookAt(
		glm::dvec3{ 8.0, 8.0, 12.0 },
		glm::dvec3{ 0.0, 0.0, 0.0 },
		glm::dvec3{ 0.0, 0.0, 1.0 });
};