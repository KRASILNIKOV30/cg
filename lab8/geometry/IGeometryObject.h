#pragma once
#include "Ray.h"
#include "intersection/Intersection.h"
#include "matrix/Matrix4.h"
#include <memory>

/*
Интерфейс "Геометрический объект"
Предоставляет возможность задания трансформирующей матрицы и возможность нахождения точек пересечения с лучом
*/
class IGeometryObject
{
public:
	[[nodiscard]] virtual Matrix4d const& GetTransform() const = 0;
	virtual void SetTransform(Matrix4d const& transform) = 0;
	virtual bool Hit(Ray const& ray, Intersection& intersection) const = 0;

	virtual ~IGeometryObject() = default;
};

using IGeometryObjectPtr = std::shared_ptr<IGeometryObject>;