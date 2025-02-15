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

	void DrawCircleWithThickness(wxImage& image, Point center, int inner, int outer, Color color)
	{
		const auto xc = center.x;
		const auto yc = center.y;
		int xo = outer;
		int xi = inner;
		int y = 0;
		int erro = 1 - xo;
		int erri = 1 - xi;

		while (xo >= y)
		{
			xLine(image, xc + xi, xc + xo, yc + y, color);
			yLine(image, xc + y, yc + xi, yc + xo, color);
			xLine(image, xc - xo, xc - xi, yc + y, color);
			yLine(image, xc - y, yc + xi, yc + xo, color);
			xLine(image, xc - xo, xc - xi, yc - y, color);
			yLine(image, xc - y, yc - xo, yc - xi, color);
			xLine(image, xc + xi, xc + xo, yc - y, color);
			yLine(image, xc + y, yc - xo, yc - xi, color);

			y++;

			if (erro < 0)
			{
				erro += 2 * y + 1;
			}
			else
			{
				xo--;
				erro += 2 * (y - xo + 1);
			}

			if (y > inner)
			{
				xi = y;
			}
			else
			{
				if (erri < 0)
				{
					erri += 2 * y + 1;
				}
				else
				{
					xi--;
					erri += 2 * (y - xi + 1);
				}
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