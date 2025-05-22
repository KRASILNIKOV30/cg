#pragma once

class ViewPort
{
public:
	/*
	Инициализация видового порта нулевых размеров в координатами верхнего левого угла (0, 0)
	*/
	ViewPort() noexcept
		: m_left(0), m_top(0), m_width(0), m_height(0)
	{
	}

	/*
	Инициализация видового порта с заданными размерами и положением
	*/
	ViewPort(unsigned left, unsigned top, unsigned width, unsigned height) noexcept
		: m_left(left), m_top(top), m_width(width), m_height(height)
	{
	}

	// Проверяем, находится ли точка внутри видового порта
	[[nodiscard]] bool TestPoint(int x, int y) const noexcept
	{
		unsigned ux = x;
		unsigned uy = y;

		return
			(ux >= m_left && ux < GetRight()) &&
			(uy >= m_top && uy < GetBottom());

	}

	[[nodiscard]] unsigned GetLeft() const noexcept
	{
		return m_left;
	}

	[[nodiscard]] unsigned GetTop() const noexcept
	{
		return m_top;
	}

	[[nodiscard]] unsigned GetBottom() const noexcept
	{
		return m_top + m_height;
	}

	[[nodiscard]] unsigned GetRight() const noexcept
	{
		return m_left + m_width;
	}

	[[nodiscard]] unsigned GetWidth() const noexcept
	{
		return m_width;
	}

	[[nodiscard]] unsigned GetHeight() const noexcept
	{
		return m_height;
	}

private:
	unsigned m_left;
	unsigned m_top;
	unsigned m_width;
	unsigned m_height;
};