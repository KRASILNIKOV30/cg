#pragma once
#include <wx/wx.h>

struct Size
{
	int width;
	int height;
};

struct Point
{
	int x;
	int y;
};

struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class Painter
{
public:
	explicit Painter(Size canvasSize)
		: m_canvasSize(canvasSize)
	{
	}

	void DrawCircle(wxImage& image, const Point center, const int radius, Color color)
	{
		const auto [r, g, b] = color;
		int x = 0;
		int y = radius;
		int delta = 3 - 2 * y;

		while (y >= x)
		{
			SetPixel(image, center.x + x, center.y + y, r, g, b);
			SetPixel(image, center.x - x, center.y + y, r, g, b);
			SetPixel(image, center.x + x, center.y - y, r, g, b);
			SetPixel(image, center.x - x, center.y - y, r, g, b);
			SetPixel(image, center.x + y, center.y + x, r, g, b);
			SetPixel(image, center.x - y, center.y + x, r, g, b);
			SetPixel(image, center.x + y, center.y - x, r, g, b);
			SetPixel(image, center.x - y, center.y - x, r, g, b);

			delta += delta < 0
				? 4 * x + 6
				: 4 * (x - y--) + 10;
			++x;
		}
	}

	void DrawCircleWithAntialiasing(wxImage& image, const Point center, const int radius, Color color)
	{
		const auto [r, g, b] = color;
		int x = 0;
		int y = radius;
		int delta = 3 - 2 * y;

		while (y >= x)
		{
			DrawPixelWithAntialiasing(image, center.x + x, center.y + y, r, g, b, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x - x, center.y + y, r, g, b, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x + x, center.y - y, r, g, b, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x - x, center.y - y, r, g, b, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x + y, center.y + x, r, g, b, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x - y, center.y + x, r, g, b, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x + y, center.y - x, r, g, b, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x - y, center.y - x, r, g, b, center.x, center.y, radius);

			delta += delta < 0
				? 4 * x + 6
				: 4 * (x - y--) + 10;
			++x;
		}
	}

private:
	void DrawPixelWithAntialiasing(wxImage& image, int x, int y, int r, int g, int b, int cx, int cy, int radius)
	{
		const auto [width, height] = m_canvasSize;
		if (x < 0 || x >= width || y < 0 || y >= height)
		{
			return;
		}

		// Рассчитываем расстояние до окружности (чем меньше расстояние, тем ярче пиксель)
		float distance = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
		float alpha = std::max(0.0f, 1.0f - std::abs(distance - radius) * 1.5f); // плавное уменьшение яркости

		// Если пиксель близко к окружности, то применяем антиалиасинг
		if (alpha > 0.0f)
		{
			// Учитываем градацию альфа-прозрачности (для сглаживания)
			int finalR = static_cast<int>(r * alpha);
			int finalG = static_cast<int>(g * alpha);
			int finalB = static_cast<int>(b * alpha);

			SetPixel(image, x, y, finalR, finalG, finalB);
		}
	}

	void SetPixel(wxImage& image, int x, int y, int r, int g, int b)
	{
		const auto [width, height] = m_canvasSize;
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			image.SetRGB(x, y, r, g, b);
		}
	}

private:
	Size m_canvasSize;
};