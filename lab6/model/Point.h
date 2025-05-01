#pragma once

struct Point
{
	float x;
	float y;

	Point& operator+=(Point const& point)
	{
		x += point.x;
		y += point.y;
		return *this;
	}

	bool operator==(const Point& other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Point& other) const
	{
		return !(*this == other);
	}
};