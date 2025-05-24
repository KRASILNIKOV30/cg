#pragma once
#include "../geometry/GeometryObjectWithInitialTransformImpl.h"
#include <cmath>
#include <complex>
#include <unsupported/Eigen/Polynomials>

class Torus final : public GeometryObjectWithInitialTransformImpl
{
public:
	explicit Torus(double majorRadius = 1.0, double minorRadius = 0.5,
		Vector3d const& center = Vector3d(), Matrix4d const& transform = Matrix4d())
		: GeometryObjectWithInitialTransformImpl(transform)
		  , m_majorRadius(majorRadius)
		  , m_minorRadius(minorRadius)
	{
		Matrix4d initialTransform;
		initialTransform.Translate(center.x, center.y, center.z);
		SetInitialTransform(initialTransform);
	}

	bool Hit(Ray const& ray, Intersection& intersection) const override
	{
		const Ray invRay = Transform(ray, GetInverseTransform());
		return HitTorusSurface(ray, invRay, intersection);
	}

private:
	bool HitTorusSurface(Ray const& ray, Ray const& invRay, Intersection& intersection) const
	{
		const auto R = m_majorRadius;
		const auto r = m_minorRadius;
		const auto [x1, y1, z1] = invRay.GetStart();
		const auto [d1, d2, d3] = invRay.GetDirection();

		double roots[4]; // solution array for the quartic equation
		const double sum_d_sqrd = d1 * d1 + d2 * d2 + d3 * d3;
		const double e = x1 * x1 + y1 * y1 + z1 * z1 - R * R - r * r;
		const double f = x1 * d1 + y1 * d2 + z1 * d3;
		const double four_a_sqrd = 4.0 * R * R;
		const auto a = e * e - four_a_sqrd * (r * r - y1 * y1);
		const auto b = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
		const auto c = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
		const auto d = 4.0 * sum_d_sqrd * f;
		const auto e1 = sum_d_sqrd * sum_d_sqrd; // coefficient of t^4
		// find roots of the quartic equation
		const auto numRoots = SolveQuartic(a, b, c, d, e1, roots);
		bool intersected = false;
		double t = std::numeric_limits<double>::max();
		if (numRoots == 0)
		{
			return false;
		}
		// find the smallest root greater than kEpsilon, if any
		// the roots array is not sorted
		for (int j = 0; j < numRoots; j++)
			if (roots[j] > 1e-8)
			{
				intersected = true;
				if (roots[j] < t)
				{
					t = roots[j];
				}
			}
		if (!intersected)
		{
			return false;
		}

		const auto hitPoint = invRay.GetPointAtTime(t);
		intersection.AddHit(CreateHitInfo(ray, invRay, t, ComputeNormal(hitPoint)));

		return true;
	}

	[[nodiscard]] Vector3d ComputeNormal(Vector3d const& point) const
	{
		const double R = m_majorRadius;
		const double r = m_minorRadius;

		const double paramSquared = R * R + r * r;
		const auto [x, y, z] = point;
		const double sumSquared = x * x + y * y + z * z;

		return {
			4.0 * x * (sumSquared - paramSquared),
			4.0 * y * (sumSquared - paramSquared + 2.0 * R * R),
			4.0 * z * (sumSquared - paramSquared),
		};
	}

	static int SolveQuartic(double a, double b, double c, double d, double e, double roots[4])
	{
		Eigen::VectorXd coeffs(5);
		coeffs << a, b, c, d, e; // от старшего к младшему

		Eigen::PolynomialSolver<double, Eigen::Dynamic> solver;
		solver.compute(coeffs);

		int count = 0;
		constexpr double EPS = 1e-8;

		for (auto root : solver.roots())
		{
			if (std::abs(root.imag()) < EPS)
			{
				const double realRoot = root.real();

				// Уникальность (удаление повторов)
				bool isUnique = true;
				for (int j = 0; j < count; ++j)
				{
					if (std::abs(roots[j] - realRoot) < EPS)
					{
						isUnique = false;
						break;
					}
				}

				if (isUnique && count < 4)
				{
					roots[count++] = realRoot;
				}
			}
		}

		return count;
	}

	[[nodiscard]] HitInfo CreateHitInfo(Ray const& ray, Ray const& invRay, double t, Vector3d const& normalInObjectSpace) const
	{
		const Vector3d hitPoint = ray.GetPointAtTime(t);
		const Vector3d hitPointInObjectSpace = invRay.GetPointAtTime(t);
		const Vector3d hitNormal = Normalize(GetNormalMatrix() * normalInObjectSpace);

		return {
			t, *this,
			hitPoint, hitPointInObjectSpace,
			hitNormal, normalInObjectSpace };
	}

	double m_majorRadius;
	double m_minorRadius;
};