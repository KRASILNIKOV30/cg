#pragma once

#include "Mesh.h"
#include "Surface.h"

class SincSurface : public Surface
{
public:
	SincSurface(
		int columns, int rows, float xMin, float xMax, float yMin, float yMax);

protected:
	// переопределяем метод вычисления вершины сетки
	[[nodiscard]] Vertex CalculateVertex(double x, double y) const override;

private:
	// функция sinc=sin(x)/x
	static double Sinc(double x);
	// Представление поверхности виде функции F(x,y,z)=f(x, y) - z
	static double F(double x, double y, double z);
};