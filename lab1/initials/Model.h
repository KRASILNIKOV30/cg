#pragma once
#include "../signals/SignallingValue.h"

#include <iostream>

constexpr int ACCELERATION = 100;

struct Size
{
	double width;
	double height;
};

struct Point
{
	double x;
	double y;
};

struct Letter
{
	Point position{};
	Size size = { 80, 100 };
	double velocity = 0;
};

class Model
{
public:
	using UpdateSignal = EmptySignal;
	using UpdateSlot = EmptySignal::slot_type;

	[[nodiscard]] Letter GetNameLetter() const
	{
		return m_name;
	}

	[[nodiscard]] Letter GetSurnameLetter() const
	{
		return m_surname;
	}

	[[nodiscard]] Letter GetPatronymicLetter() const
	{
		return m_patronymic;
	}

	void Update(double const deltaTime)
	{
		UpdateLetter(m_name, deltaTime);
		UpdateLetter(m_surname, deltaTime);
		UpdateLetter(m_patronymic, deltaTime);

		m_canvasUpdateSignal();
	}

	[[nodiscard]] ScopedConnection DoOnUpdate(UpdateSlot const& slot)
	{
		return m_canvasUpdateSignal.connect(slot);
	}

	[[nodiscard]] Size GetCanvasSize() const
	{
		return m_canvasSize;
	}

private:
	void UpdateLetter(Letter& letter, double const deltaTime)
	{
		if (letter.position.y + letter.size.height >= m_canvasSize.height && letter.velocity >= 0)
		{
			letter.velocity *= -1;
		}
		else
		{
			letter.velocity += ACCELERATION * deltaTime;
			letter.position.y += letter.velocity * deltaTime;
		}
	}

private:
	Letter m_surname = { 150, 200 };
	Letter m_name = { 325, 100 };
	Letter m_patronymic = { 500, 300 };
	Size m_canvasSize = { 800, 600 };
	UpdateSignal m_canvasUpdateSignal;
};