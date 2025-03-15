#pragma once
#include "../BaseWindow.h"
#include "../model/Model.h"
#include "../view/View.h"
#include <chrono>
#include <GLFW/glfw3.h>

using namespace std::chrono;

class Controller final : public BaseWindow
{
	using BaseWindow::BaseWindow;

public:
	Controller(Model& model, View& view)
		: BaseWindow(800, 800, "tetris")
		  , m_model(model)
		  , m_view(view)
	{
		m_keyState[GLFW_KEY_LEFT] = false;
		m_keyState[GLFW_KEY_RIGHT] = false;
		m_keyState[GLFW_KEY_UP] = false;
	}

	void OnRunStart() override
	{
		Update();
	}

	void Draw(int width, int height) override
	{
		const auto currentTime = steady_clock::now();
		const auto deltaTime = duration_cast<duration<double>>(currentTime - m_lastUpdateTime).count();

		ProcessInput();

		if (deltaTime >= m_model.GetUpdateTime())
		{
			Update();
		}

		m_view.Render(width, height);
		glfwPollEvents();
	}

private:
	void Update()
	{
		m_model.Update();
		m_lastUpdateTime = steady_clock::now();
	}

	void ProcessInput()
	{
		const auto window = GetWindow();

		CheckKeyPress(window, GLFW_KEY_LEFT, [&] { m_model.MoveLeft(); });
		CheckKeyPress(window, GLFW_KEY_RIGHT, [&] { m_model.MoveRight(); });
		CheckKeyPress(window, GLFW_KEY_UP, [&] { m_model.Rotate(); });
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			// Сделать автоповтор для снижения фигуры
			m_model.MoveDown();
		}
	}

	void CheckKeyPress(GLFWwindow* window, int key, std::function<void()> const& callback)
	{
		bool keyPressed = glfwGetKey(window, key) == GLFW_PRESS;
		if (keyPressed && !m_keyState[key])
		{
			callback();
			m_keyState[key] = true;
		}
		else if (!keyPressed)
		{
			m_keyState[key] = false;
		}
	}

private:
	Model& m_model;
	View& m_view;
	steady_clock::time_point m_lastUpdateTime;
	bool m_keyState[GLFW_KEY_LAST] = { false };
};
