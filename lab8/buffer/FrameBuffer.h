#pragma once
#include <cassert>
#include <cstdint>
#include <vector>

/*
	Класс FrameBuffer, представляющий собой 32-битный буфер кадра
	фиксированного размера
*/
class FrameBuffer
{
public:
	FrameBuffer(unsigned const width, unsigned const height)
		: m_pixels(width * height)
		  , m_width(width)
		  , m_height(height)
	{
	}


	// Ширина буфера в пикселях
	[[nodiscard]] unsigned GetWidth() const noexcept
	{
		return m_width;
	}

	// Высота буфера в пикселях
	[[nodiscard]] unsigned GetHeight() const noexcept
	{
		return m_height;
	}

	// Очистка содержимого буфера заданным цветом
	void Clear(const std::uint32_t color = 0)
	{
		std::ranges::fill(m_pixels, color);
	}

	// Получение адреса начала соотв. строки пикселей (для чтения)
	[[nodiscard]] const std::uint32_t* GetPixels(unsigned row = 0) const noexcept
	{
		assert(row < m_height);
		return &m_pixels[row * m_width];
	}

	// Получение адреса начала соотв. строки пикселей (для записи)
	std::uint32_t* GetPixels(const unsigned row = 0) noexcept
	{
		assert(row < m_height);
		return &m_pixels[size_t(row * m_width)];
	}

	// Получение цвета пикселя с заданными координатами
	[[nodiscard]] std::uint32_t GetPixel(const unsigned x, const unsigned y) const noexcept
	{
		assert(x < m_width);
		assert(y < m_height);
		return m_pixels[y * m_width + x];
	}

	// Установка цвета пикселя с заданными координатами
	void SetPixel(unsigned x, unsigned y, std::uint32_t color) noexcept
	{
		assert(x < m_width);
		assert(y < m_height);
		m_pixels[y * m_width + x] = color;
	}

private:
	std::vector<std::uint32_t> m_pixels;
	unsigned m_width;
	unsigned m_height;
};