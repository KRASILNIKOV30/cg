#pragma once
#include "../vector/VectorMath.h"

class IGeometryObject;

class HitInfo
{
public:
	// Конструктор по умолчанию - пересечения нет
	HitInfo()
		: m_hitTime(-1)
		  , m_pHitObject(nullptr)
	{
	}

	/*
		hitTime - время столкновения луча с объектом
		hitObject -  объект столкновения
		hitPoint - точка столкновения в системе координатах сцены
		hitPointInObjectSpace - точка столкновения в системе координат объекта
	*/
	HitInfo(
		const double hitTime,
		IGeometryObject const& hitObject,
		Vector3d const& hitPoint,
		Vector3d const& hitPointInObjectSpace,
		Vector3d const& normal,
		Vector3d const& normalInObjectSpace
		)
		: m_hitPoint(hitPoint)
		  , m_hitPointInObjectSpace(hitPointInObjectSpace)
		  , m_normal(Normalize(normal))
		  , m_normalInObjectSpace(Normalize(normalInObjectSpace))
		  , m_hitTime(hitTime)
		  , m_pHitObject(&hitObject)
	{
	}

	// Инициализирована ли точка пересечения
	[[nodiscard]] bool IsInitialized() const
	{
		return m_pHitObject != nullptr;
	}

	// Возвращает время столкновения луча с объектом
	[[nodiscard]] double GetHitTime() const
	{
		assert(IsInitialized());
		return m_hitTime;
	}

	// Возвращает координаты точки в мировой системе координат
	[[nodiscard]] Vector3d const& GetHitPoint() const
	{
		assert(IsInitialized());
		return m_hitPoint;
	}

	// Возвращает координаты точки в системе координат объекта
	[[nodiscard]] Vector3d const& GetHitPointInObjectSpace() const
	{
		assert(IsInitialized());
		return m_hitPointInObjectSpace;
	}

	// Ссылка на объект, с которым произошло пересечение
	[[nodiscard]] IGeometryObject const& GetHitObject() const
	{
		assert(IsInitialized());
		return *m_pHitObject;
	}

	// Нормаль к поверхности в мировой системе координат
	[[nodiscard]] Vector3d const& GetNormal() const
	{
		return m_normal;
	}

	// Нормаль к поверхности в системе координат объекта
	[[nodiscard]] Vector3d const& GetNormalInObjectSpace() const
	{
		return m_normalInObjectSpace;
	}

private:
	Vector3d m_hitPoint;
	Vector3d m_hitPointInObjectSpace;
	Vector3d m_normal;
	Vector3d m_normalInObjectSpace;
	double m_hitTime;
	IGeometryObject const* m_pHitObject{};
};