#pragma once
#include <wx/wx.h>

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

inline void DrawCircle(wxImage& image, const Point center, const int radius, Color color)
{
	const auto [r, g, b] = color;
	int x = 0;
	int y = radius;
	int delta = 3 - 2 * y;

	while (y >= x)
	{
		image.SetRGB(center.x + x, center.y + y, r, g, b);
		image.SetRGB(center.x - x, center.y + y, r, g, b);
		image.SetRGB(center.x + x, center.y - y, r, g, b);
		image.SetRGB(center.x - x, center.y - y, r, g, b);
		image.SetRGB(center.x + y, center.y + x, r, g, b);
		image.SetRGB(center.x - y, center.y + x, r, g, b);
		image.SetRGB(center.x + y, center.y - x, r, g, b);
		image.SetRGB(center.x - y, center.y - x, r, g, b);

		delta += delta < 0
			? 4 * x + 6
			: 4 * (x - y--) + 10;
		++x;
	}
}