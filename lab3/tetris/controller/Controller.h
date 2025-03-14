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
	Controller(Model& m, View& v)
		: BaseWindow(800, 800, "tetris")
		  , model(m)
		  , view(v)
	{
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

		if (deltaTime >= model.GetUpdateTime())
		{
			Update();
		}

		view.Render(width, height);
		glfwPollEvents();
	}

private:
	void Update()
	{
		model.Update();
		m_lastUpdateTime = steady_clock::now();
	}

	void ProcessInput() const
	{
		const auto window = GetWindow();
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			model.MoveLeft();
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			model.MoveRight();
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			model.MoveDown();
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			model.Rotate();
		}
	}

private:
	Model& model;
	View& view;
	steady_clock::time_point m_lastUpdateTime;
};