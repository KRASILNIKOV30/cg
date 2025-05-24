#pragma once
#include "AABB.h"
#include "../geometry/vector/VectorMath.h"
#include "../geometry/GeometryObjectWithInitialTransformImpl.h"
#include <vector>
#include <cmath>
#include <iostream>

struct Metasphere
{
	Vector3d center;
	double radius;
	double weight;
};

class Metaball final : public GeometryObjectWithInitialTransformImpl
{
public:
	explicit Metaball(std::vector<Metasphere> const& spheres,
		Vector3d const& center = Vector3d(),
		Matrix4d const& transform = Matrix4d(),
		double isoThreshold = 0.5) // Более низкий порог по умолчанию
		: GeometryObjectWithInitialTransformImpl(transform)
		  , m_spheres(spheres)
		  , m_isoThreshold(isoThreshold)
	{
		Matrix4d initialTransform;
		initialTransform.Translate(center.x, center.y, center.z);
		SetInitialTransform(initialTransform);
		ComputeBoundingBox();
	}

	bool Hit(Ray const& ray, Intersection& intersection) const override
	{
		const Ray invRay = Transform(ray, GetInverseTransform());
		double tMin, tMax;
		if (!m_aabb.Intersect(invRay, tMin, tMax))
		{
			return false;
		}

		const double stepSize = 0.05;
		const int maxSteps = 100;
		double t = tMin;

		for (int i = 0; i < maxSteps && t < tMax; ++i)
		{
			Vector3d p = invRay.GetPointAtTime(t);
			double field = EvaluateField(p);

			if (field > m_isoThreshold)
			{
				// Уточнение пересечения бинарным поиском
				double t0 = t - stepSize;
				double t1 = t;
				for (int j = 0; j < 5; ++j)
				{
					double tm = (t0 + t1) * 0.5;
					if (EvaluateField(invRay.GetPointAtTime(tm)) > m_isoThreshold)
					{
						t1 = tm;
					}
					else
					{
						t0 = tm;
					}
				}
				t = (t0 + t1) * 0.5;

				// Вычисляем нормаль
				Vector3d normal = ComputeNormal(invRay.GetPointAtTime(t));

				intersection.AddHit(CreateHitInfo(ray, invRay, t, normal));
				return true;
			}

			t += stepSize;
		}

		return false;
	}

private:
	std::vector<Metasphere> m_spheres;
	double m_isoThreshold;
	AABB m_aabb;

	void ComputeBoundingBox()
	{
		Vector3d min(1e10, 1e10, 1e10);
		Vector3d max(-1e10, -1e10, -1e10);

		for (const auto& s : m_spheres)
		{
			Vector3d r(s.radius, s.radius, s.radius);
			min = Min(min, s.center - r);
			max = Max(max, s.center + r);
		}
		m_aabb = AABB(min, max);
	}

	[[nodiscard]] double EvaluateField(Vector3d const& p) const
	{
		double sum = 0.0;
		for (const auto& s : m_spheres)
		{
			double r2 = (p - s.center).GetLengthSquared();
			double R2 = s.radius * s.radius;

			if (r2 < R2)
			{
				const double ratio = r2 / R2;
				sum += s.weight * (1 - 3 * ratio + 3 * ratio * ratio - ratio * ratio * ratio);
			}
		}
		return sum;
	}

	[[nodiscard]] Vector3d ComputeNormal(Vector3d const& p) const
	{
		const double eps = 1e-3; // Чуть больше — стабильнее

		const double fx1 = EvaluateField(p + Vector3d(eps, 0, 0));
		const double fx2 = EvaluateField(p - Vector3d(eps, 0, 0));
		const double fy1 = EvaluateField(p + Vector3d(0, eps, 0));
		const double fy2 = EvaluateField(p - Vector3d(0, eps, 0));
		const double fz1 = EvaluateField(p + Vector3d(0, 0, eps));
		const double fz2 = EvaluateField(p - Vector3d(0, 0, eps));

		return -Normalize(Vector3d{
			(fx1 - fx2) * 0.5,
			(fy1 - fy2) * 0.5,
			(fz1 - fz2) * 0.5
		});
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
};
