#include "MobiusStrip.h"

MobiusStrip::MobiusStrip(int columns, int rows, float uMin, float uMax, float vMin, float vMax)
	: Surface(columns, rows, uMin, uMax, vMin, vMax)
{
}

Vertex MobiusStrip::CalculateVertex(double u, double v) const
{
	double x = X(u, v);
	double y = Y(u, v);
	double z = Z(u, v);

	double dx_du, dx_dv, dy_du, dy_dv, dz_du, dz_dv;
	CalculatePartialDerivatives(u, v, dx_du, dx_dv, dy_du, dy_dv, dz_du, dz_dv);

	double nx = dy_du * dz_dv - dz_du * dy_dv;
	double ny = dz_du * dx_dv - dx_du * dz_dv;
	double nz = dx_du * dy_dv - dy_du * dx_dv;

	double length = sqrt(nx * nx + ny * ny + nz * nz);
	nx /= length;
	ny /= length;
	nz /= length;

	return {
		{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) },
		{ static_cast<float>(nx), static_cast<float>(ny), static_cast<float>(nz) }
	};
}

double MobiusStrip::X(double u, double v) const
{
	return (1 + v / 2 * cos(u / 2)) * cos(u);
}

double MobiusStrip::Y(double u, double v) const
{
	return (1 + v / 2 * cos(u / 2)) * sin(u);
}

double MobiusStrip::Z(double u, double v) const
{
	return v / 2 * sin(u / 2);
}

void MobiusStrip::CalculatePartialDerivatives(double u, double v,
	double& dx_du, double& dx_dv,
	double& dy_du, double& dy_dv,
	double& dz_du, double& dz_dv) const
{
	dx_du = (X(u + epsilon, v) - X(u - epsilon, v)) / (2 * epsilon);
	dx_dv = (X(u, v + epsilon) - X(u, v - epsilon)) / (2 * epsilon);

	dy_du = (Y(u + epsilon, v) - Y(u - epsilon, v)) / (2 * epsilon);
	dy_dv = (Y(u, v + epsilon) - Y(u, v - epsilon)) / (2 * epsilon);

	dz_du = (Z(u + epsilon, v) - Z(u - epsilon, v)) / (2 * epsilon);
	dz_dv = (Z(u, v + epsilon) - Z(u, v - epsilon)) / (2 * epsilon);
}