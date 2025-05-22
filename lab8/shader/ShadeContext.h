#pragma once
#include "../geometry/vector/Vector4.h"
class Ray;
class Scene;

/*
Контекст закрашивания, используемый шейдером для вычисления цвета поверхности
Хранит информацию о координатах обрабатываемой точки, нормали и направлении луча, а также ссылку на сцену
(в дальнейшем понадобится для построения теней, а также расчета преломлений/отражений)
*/
class ShadeContext
{
public:
	/*
	Инициализирует контекст закрашивания
	*/
	ShadeContext(
		Scene const& scene,
		Vector3d const& surfacePoint,
		Vector3d const& surfacePointInObjectSpace,
		Vector3d const& surfaceNormal, // нормаль в мировой системе координат
		Vector3d const& rayDirection // направление трассируемого луча в мировой системе координат
		) noexcept
		: m_surfacePoint(surfacePoint)
		  , m_surfacePointInObjectSpace(surfacePointInObjectSpace)
		  , m_surfaceNormal(surfaceNormal)
		  , m_rayDirection(rayDirection)
		  , m_scene(scene)
	{
	}

	/*
	Возвращает координаты точки в мировой системе координат.
	*/
	[[nodiscard]] Vector3d const& GetSurfacePoint() const noexcept
	{
		return m_surfacePoint;
	}

	/*
	Возвращает координаты точки в системе координат объекта
	*/
	[[nodiscard]] Vector3d const& GetSurfacePointInObjectSpace() const noexcept
	{
		return m_surfacePointInObjectSpace;
	}

	/*
	Возвращает нормаль к поверхности в мировой системе координат
	*/
	[[nodiscard]] Vector3d const& GetSurfaceNormal() const noexcept
	{
		return m_surfaceNormal;
	}

	/*
	Возвращает направление луча, попавшего в данную точку
	*/
	[[nodiscard]] Vector3d const& GetRayDirection() const noexcept
	{
		return m_rayDirection;
	}

	/*
	Возвращает ссылку на сцену
	*/
	[[nodiscard]] Scene const& GetScene() const noexcept
	{
		return m_scene;
	}

private:
	Vector3d const& m_surfacePoint;
	Vector3d const& m_surfacePointInObjectSpace;
	Vector3d const& m_surfaceNormal;
	Vector3d const& m_rayDirection;
	Scene const& m_scene;
};