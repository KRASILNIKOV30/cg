#pragma once
#include "../geometry/Ray.h"

class AABB
{
public:
	Vector3d min;
	Vector3d max;

	AABB() = default;

	AABB(const Vector3d& min_, const Vector3d& max_)
		: min(min_), max(max_)
	{
	}

	bool Intersect(const Ray& ray, double& tminOut, double& tmaxOut) const
	{
		const Vector3d& origin = ray.GetStart();
		const Vector3d& dir = ray.GetDirection();

		double tmin = (min.x - origin.x) / dir.x;
		double tmax = (max.x - origin.x) / dir.x;
		if (tmin > tmax)
			std::swap(tmin, tmax);

		double tymin = (min.y - origin.y) / dir.y;
		double tymax = (max.y - origin.y) / dir.y;
		if (tymin > tymax)
			std::swap(tymin, tymax);

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		double tzmin = (min.z - origin.z) / dir.z;
		double tzmax = (max.z - origin.z) / dir.z;
		if (tzmin > tzmax)
			std::swap(tzmin, tzmax);

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		tminOut = tmin;
		tmaxOut = tmax;
		return true;
	}
};
