//
// Created by bogdan.krasilnikov on 04.04.25.
//

#include "SincSurface.h"

SincSurface::SincSurface(
	int columns, int rows, float xMin, float xMax, float yMin, float yMax)
	: Surface(columns, rows, xMin, xMax, yMin, yMax)
{
}

Vertex SincSurface::CalculateVertex(double x, double y) const
{
	// вычисляем значение координаты z
	double r = sqrt(x * x + y * y);
	double z = Sinc(r);

	// "бесконечно малое" приращение аргумента
	// для численного дифференцирования
	double delta = 1e-6;

	// Вычисляем значение функции в точке x, y, z
	// (строго говоря, оно должно быть равно нулю в точках,
	// принадлежащих поверхности)
	float f = F(x, y, z);

	// вычисляем приблизительно частные производные функции F по dx, dy и dz
	// их значения приблизительно равны координатам вектора
	// нормали к поверхности а точке (x, y, z)
	double dfdx = -(F(x + delta, y, z) - f) / delta;
	double dfdy = -(F(x, y + delta, z) - f) / delta;
	double dfdz = 1; // производная функции -F(x,y,z)=-(f(x,y)-z) по z

	// величина, обратная длине векора антиградиента
	double invLen = 1 / sqrt(dfdx * dfdx + dfdy * dfdy + dfdz * dfdz);

	// формируем результат
	Vertex result =
	{
		// координаты вершины
		{ float(x), float(y), float(z) },

		// приводим вектор нормали к единичной длине
		{ float(dfdx * invLen), float(dfdy * invLen), float(dfdz * invLen) }
	};
	return result;
}

double SincSurface::Sinc(double x)
{
	// Вычисляем первый замечательный предел
	return (fabs(x) < 1e-7) ? 1 : sin(x) / x;
}

double SincSurface::F(double x, double y, double z)
{
	double r = sqrt(x * x + y * y);
	double f = Sinc(r);
	return f - z;
}