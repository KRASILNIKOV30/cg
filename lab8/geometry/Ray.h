#pragma once
#include "vector/Vector3.h"

class Ray
{
public:
	Ray(Vector3d const& start, Vector3d const& direction) noexcept
		: m_start(start), m_direction(direction)
	{
	}

	Vector3d GetPointAtTime(const double time) const noexcept
	{
		return m_start + m_direction * time;
	}

	Vector3d const& GetStart() const noexcept
	{
		return m_start;
	}

	Vector3d const& GetDirection() const noexcept
	{
		return m_direction;
	}

private:
	Vector3d m_start;
	Vector3d m_direction;
};

#include "vector/VectorMath.h"
#include "matrix/Matrix4.h"

/*
Трансформация луча с использованием заданной матрицы
*/
inline Ray Transform(Ray const& ray, Matrix4d const& matrix) noexcept
{
	const Vector3d start = (matrix * Vector4d(ray.GetStart(), 1)).Project();
	const Vector3d direction{ matrix * Vector4d(ray.GetDirection(), 0) };
	return Ray(start, direction);
}