#pragma once
#include "LightSourceImpl.h"
#include "../geometry/vector/Vector4.h"
#include "../geometry/vector/VectorMath.h"
#include "../random/Random.h"

/*
Класс "Точечный источник света", характеризующийся позицией в пространстве.
Для данного типа источника света можно задать коэффициенты ослабления света в зависимости
от расстояния до объекта
*/
class OmniLightSource final : public LightSourceImpl
{
public:
	explicit OmniLightSource(
		Vector3d const& position = Vector3d(),
		Matrix4d const& transform = Matrix4d())
		: LightSourceImpl(transform)
		  , m_position(position)
		  , m_constantAttenuation(1)
		  , m_linearAttenuation(0)
		  , m_quadraticAttenuation(0)
	{
		UpdatePositionInWorldSpace();
	}

	/*
	Перегруженный метод установки матрицы трансформации
	*/
	void SetTransform(Matrix4d const& transform) override
	{
		LightSourceImpl::SetTransform(transform);
		UpdatePositionInWorldSpace();
	}

	/*
	Вычисление направления из точки пространства к источнику света
	*/
	[[nodiscard]] Vector3d GetDirectionFromPoint(Vector3d const& point) const override
	{
		/*
			Направление от точки к источнику в мировых координатах
		*/
		return GetPositionInWorldSpace() - point;
	}

	[[nodiscard]] Vector3d GetRandomDirectionFromPoint(Vector3d const& point) const override
	{
		const double theta = 2.0 * M_PI * RandomDouble();
		const double phi = std::acos(1.0 - 2.0 * RandomDouble());

		const double x = m_radius * std::sin(phi) * std::cos(theta);
		const double y = m_radius * std::sin(phi) * std::sin(theta);
		const double z = m_radius * std::cos(phi);

		return m_positionInWorldSpace + Vector3d(x, y, z) - point;
	}

	/*
	Вычисление интенсивности света, излучаемой в заданном направлении
	*/
	[[nodiscard]] double GetIntensityInDirection(Vector3d const& direction) const override
	{
		/*
		Вычисляем расстояние (длина вектора направления)
		*/
		const double distance = direction.GetLength();

		/*
		Вычисляем интенсивность света с учетом коэффициентов ослабления
		*/
		return 1.0 / (distance * distance * m_quadraticAttenuation + distance * m_linearAttenuation + m_constantAttenuation);
	}

	/*
	Установка коэффициентов ослабления излучаемой интенсивности в зависимости от расстояния
	*/
	void SetAttenuation(
		double const& constantAttenuation,
		double const& linearAttenuation,
		double const& quadraticAttenuation)
	{
		m_constantAttenuation = constantAttenuation;
		m_linearAttenuation = linearAttenuation;
		m_quadraticAttenuation = quadraticAttenuation;
	}

private:
	// Получение позиции источника света в мировых координатах
	[[nodiscard]] Vector3d const& GetPositionInWorldSpace() const
	{
		return m_positionInWorldSpace;
	}

	//	Обновление позиции источника света в мировых координатах в зависимости от матрицы трансформации
	void UpdatePositionInWorldSpace()
	{
		m_positionInWorldSpace = Vector3d{ GetTransform() * Vector4d(m_position, 1) };
	}

private:
	// Начальная позиция источника света
	Vector3d m_position;

	/*
	Вычисленная и закешированная позиция источника света в мировых координатах
	*/
	Vector3d m_positionInWorldSpace;

	// Коэффициенты ослабления света в зависимости от расстояния
	double m_constantAttenuation;
	double m_linearAttenuation;
	double m_quadraticAttenuation;
	double m_radius = 0.5;
};

using OmniLightPtr = std::shared_ptr<OmniLightSource>;