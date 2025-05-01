#pragma once
#include <complex>

template <class T> class Vector3
{
public:
	T x, y, z;

	Vector3()
		: x(0), y(0), z(0)
	{
	}

	explicit Vector3(const T* p)
		: x(p[0]), y(p[1]), z(p[2])
	{
	}

	Vector3(T x0, T y0, T z0)
		: x(x0), y(y0), z(z0)
	{
	}

	explicit operator T const*() const
	{
		return &x;
	}

	Vector3& operator *=(T scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	Vector3 operator-(Vector3 const& v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator+(Vector3 const& v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator*(T scale) const
	{
		return Vector3(x * scale, y * scale, z * scale);
	}

	T GetLength() const
	{
		return static_cast<T>(std::sqrt(x * x + y * y + z * z));
	}

	// Нормализуем
	void Normalize()
	{
		float invLength = 1 / GetLength();
		x *= invLength;
		y *= invLength;
		z *= invLength;
	}
};


// Вычисляем векторное произведение a и b
template <class T> Vector3<T> Cross(Vector3<T> const& a, Vector3<T> const& b)
{
	/*
	Векторное произведение векторов A и B равно определителю матрицы:
		  |i  j  k |
	AxB = |Ax Ay Az|
		  |Bx By Bz|
	*/
	return Vector3<T>(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);

}

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;