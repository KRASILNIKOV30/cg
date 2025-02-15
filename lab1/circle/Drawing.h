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
		int x = 0;
		int y = radius;
		int delta = 3 - 2 * y;

		while (y >= x)
		{
			SetPixel(image, center.x + x, center.y + y, color);
			SetPixel(image, center.x - x, center.y + y, color);
			SetPixel(image, center.x + x, center.y - y, color);
			SetPixel(image, center.x - x, center.y - y, color);
			SetPixel(image, center.x + y, center.y + x, color);
			SetPixel(image, center.x - y, center.y + x, color);
			SetPixel(image, center.x + y, center.y - x, color);
			SetPixel(image, center.x - y, center.y - x, color);

			delta += delta < 0
				? 4 * x + 6
				: 4 * (x - y--) + 10;
			++x;
		}
	}

	void DrawCircleWithAntialiasing(wxImage& image, const Point center, const int radius, Color color)
	{
		int x = 0;
		int y = radius;
		int delta = 3 - 2 * y;

		while (y >= x)
		{
			DrawPixelWithAntialiasing(image, center.x + x, center.y + y, color, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x - x, center.y + y, color, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x + x, center.y - y, color, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x - x, center.y - y, color, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x + y, center.y + x, color, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x - y, center.y + x, color, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x + y, center.y - x, color, center.x, center.y, radius);
			DrawPixelWithAntialiasing(image, center.x - y, center.y - x, color, center.x, center.y, radius);

			delta += delta < 0
				? 4 * x + 6
				: 4 * (x - y--) + 10;
			++x;
		}
	}

	void xLine(wxImage& image, int x1, int x2, int y, Color color)
	{
		while (x1 <= x2)
		{
			SetPixel(image, x1++, y, color);
		}
	}

	void yLine(wxImage& image, int x, int y1, int y2, Color color)
	{
		while (y1 <= y2)
		{
			SetPixel(image, x, y1++, color);
		}
	}

	void DrawFillCircle(wxImage& image, const Point center, const int radius, Color color)
	{
		DrawCircleWithThickness(image, center, 0, radius, color);
	}

	void DrawCircleWithThickness(wxImage& image, Point center, int inner, int outer, Color color)
	{
		const auto xc = center.x;
		const auto yc = center.y;
		int xOut = outer;
		int xIn = inner;
		int y = 0;
		int deltaOut = 1 - xOut;
		int deltaIn = 1 - xIn;

		while (xOut >= y)
		{
			xLine(image, xc + xIn, xc + xOut, yc + y, color);
			yLine(image, xc + y, yc + xIn, yc + xOut, color);
			xLine(image, xc - xOut, xc - xIn, yc + y, color);
			yLine(image, xc - y, yc + xIn, yc + xOut, color);
			xLine(image, xc - xOut, xc - xIn, yc - y, color);
			yLine(image, xc - y, yc - xOut, yc - xIn, color);
			xLine(image, xc + xIn, xc + xOut, yc - y, color);
			yLine(image, xc + y, yc - xOut, yc - xIn, color);

			deltaOut += deltaOut < 0
				? 4 * y + 6
				: 4 * (y - xOut--) + 10;
			++y;

			if (y > inner)
			{
				xIn = y;
			}
			else
			{
				deltaIn += deltaIn < 0
					? 4 * y + 6
					: 4 * (y - xIn--) + 10;
			}
		}
	}

private:
	void DrawPixelWithAntialiasing(wxImage& image, int x, int y, Color color, int cx, int cy, int radius)
	{
		const auto [width, height] = m_canvasSize;

		const auto distance = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
		const float alpha = std::max<float>(0, 1 - std::abs(distance - radius) * 1.5);

		SetPixel(image, x, y, color, alpha);
	}

	void SetPixel(wxImage& image, int x, int y, Color color, float alpha = 1.0)
	{
		const auto [width, height] = m_canvasSize;
		const auto [r, g, b] = color;
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			image.SetRGB(x, y, r * alpha, g * alpha, b * alpha);
		}
	}

private:
	Size m_canvasSize;
};