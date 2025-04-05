#pragma once
#include "Surface.h"

class MobiusStrip final : public Surface
{
public:
	MobiusStrip(int columns, int rows, float uMin, float uMax, float vMin, float vMax);

	Vertex CalculateVertex(double u, double v) const override;

private:
	double X(double u, double v) const;
	double Y(double u, double v) const;
	double Z(double u, double v) const;
	void CalculatePartialDerivatives(double u, double v,
		double& dx_du, double& dx_dv,
		double& dy_du, double& dy_dv,
		double& dz_du, double& dz_dv) const;

	const double epsilon = 1e-6;
};