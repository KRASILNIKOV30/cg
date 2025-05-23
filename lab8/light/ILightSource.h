#pragma once
#include "../geometry/matrix/Matrix4.h"
#include "../geometry/vector/Vector4.h"

#include <memory>

/*
Интерфейс "Источник света"
*/
class ILightSource
{
public:
	/*
	Установка и получение матрицы трансформации источника света
	*/
	virtual void SetTransform(Matrix4d const& transform) = 0;
	[[nodiscard]] virtual Matrix4d const& GetTransform() const = 0;

	/*
	Установка и получение интенсивности диффузного света
	*/
	virtual void SetDiffuseIntensity(Vector4f const& diffuseIntensity) = 0;
	[[nodiscard]] virtual Vector4f const& GetDiffuseIntensity() const = 0;

	virtual void SetAmbientIntensity(Vector4f const& ambientIntensity) = 0;
	[[nodiscard]] virtual Vector4f const& GetAmbientIntensity() const = 0;

	virtual void SetSpecularIntensity(Vector4f const& specularIntensity) = 0;
	[[nodiscard]] virtual Vector4f const& GetSpecularIntensity() const = 0;

	/*
	Интенсивность света, излучаемого в направлении от источника к заданной точке, заданной в мировой системе координат.
	Для точечных источников позволяет задать ослабление света в зависимости от расстояния
	Для конических источников света попутно позволяет задать ослабление излучаемого света в зависимости
	от угла отклонения от вектора направления
	*/
	[[nodiscard]] virtual double GetIntensityInDirection(Vector3d const& direction) const = 0;

	/*
	Направление на источник света из указанной точки, заданной в мировой системе координат
	*/
	[[nodiscard]] virtual Vector3d GetDirectionFromPoint(Vector3d const& point) const = 0;

	virtual ~ILightSource() = default;
};

using ILightSourcePtr = std::shared_ptr<ILightSource>;