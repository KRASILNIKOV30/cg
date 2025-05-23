#pragma once
#include "../geometry/GeometryObjectWithInitialTransformImpl.h"
#include <cmath>

class Paraboloid final : public GeometryObjectWithInitialTransformImpl
{
public:
	explicit Paraboloid(double scale = 1.0, Vector3d const& center = Vector3d(), Matrix4d const& transform = Matrix4d())
		: GeometryObjectWithInitialTransformImpl(transform)
	{
		Matrix4d initialTransform;
		initialTransform.Translate(center.x, center.y, center.z);
		initialTransform.Scale(scale, scale, scale);
		SetInitialTransform(initialTransform);
	}

	bool Hit(Ray const& ray, Intersection& intersection) const override
	{
		const Ray invRay = Transform(ray, GetInverseTransform());

		const auto hitLid = HitLid(ray, invRay, intersection);
		const auto hitSurface = HitParaboloidSurface(ray, invRay, intersection);

		return hitLid || hitSurface;
	}

private:
	bool HitParaboloidSurface(Ray const& ray, Ray const& invRay, Intersection& intersection) const
	{
		const Vector3d& S = invRay.GetStart();
		const Vector3d& D = invRay.GetDirection();

		const double a = D.x * D.x + D.y * D.y;
		const double b = 2 * (S.x * D.x + S.y * D.y) - D.z;
		const double c = S.x * S.x + S.y * S.y - S.z;
		const double disc = b * b - 4 * a * c;

		if (disc < 0)
		{
			return false;
		}

		const double sqrtDisc = sqrt(disc);
		const double t1 = (-b - sqrtDisc) / (2 * a);
		const double t2 = (-b + sqrtDisc) / (2 * a);

		bool hasHit = false;
		constexpr double HIT_TIME_EPSILON = 1e-8;

		auto checkAndAddHit = [&](double t) {
			if (t > HIT_TIME_EPSILON)
			{
				const Vector3d hitPoint = invRay.GetPointAtTime(t);
				// Отбрасываем точки с z > 1 (они принадлежат крышке)
				if (hitPoint.z <= 1.0)
				{
					// Нормаль вычисляем через градиент: (2x, 2y, -1)
					Vector3d normal(hitPoint.x * 2, hitPoint.y * 2, -1.0);
					normal.Normalize();

					intersection.AddHit(CreateHitInfo(ray, invRay, t, normal));
					hasHit = true;
				}
			}
		};

		checkAndAddHit(t1);
		checkAndAddHit(t2);

		return hasHit;
	}

	bool HitLid(Ray const& ray, Ray const& invRay, Intersection& intersection) const
	{
		// Уравнение крышки: z = 1, x² + y² ≤ 1
		const Vector3d& S = invRay.GetStart();
		const Vector3d& D = invRay.GetDirection();

		// Луч параллелен плоскости крышки
		if (std::abs(D.z) < 1e-8)
		{
			return false;
		}

		const double t = (1.0 - S.z) / D.z;
		if (t <= 1e-8)
		{
			return false;
		}

		const Vector3d hitPoint = invRay.GetPointAtTime(t);
		const double r2 = hitPoint.x * hitPoint.x + hitPoint.y * hitPoint.y;

		// Проверяем, что точка попадает в круг
		if (r2 <= 1.0)
		{
			// Нормаль к крышке всегда (0, 0, 1)
			const Vector3d normal(0, 0, 1);
			intersection.AddHit(CreateHitInfo(ray, invRay, t, normal));
			return true;
		}

		return false;
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