#pragma once
#include "../light/ILightSource.h"
#include "../geometry/vector/Vector4.h"
#include "sceneObject/SceneObject.h"

class Ray;
class Intersection;

/************************************************************************/
/* Класс "Сцена" - хранит объекты, предоставляет методы для нахождения  */
/* пересечений луча с объектами сцены                                   */
/************************************************************************/
class Scene
{
public:
	// Задать цвет заднего фона сцены
	void SetBackdropColor(Vector4f const& backdropColor);

	/*
	Добавляем объект в сцену
	*/
	void AddObject(const SceneObjectPtr& pSceneObject);

	/*
	Добавляем источник света в сцену
	*/
	void AddLightSource(const ILightSourcePtr& pLightSource);

	/*
	Возвращает количество источников света в сцене
	*/
	[[nodiscard]] size_t GetLightsCount() const;

	/*
	Доступ к источнику света с указанным индексом
	*/
	[[nodiscard]] ILightSource const& GetLight(size_t index) const;

	/*
	Возвращает цвет луча, столкнувшегося с объектами сцены
	*/
	[[nodiscard]] Vector4f Shade(Ray const& ray) const;

	/*
	Трассирует луч вглубь сцены и возвращает информацию о первом столкновении луча с объектам сцены
	*/
	bool GetFirstHit(Ray const& ray, Intersection& bestIntersection, SceneObject const** ppIntersectionObject) const;

private:
	// Коллекция объектов сцены
	typedef std::vector<SceneObjectPtr> SceneObjects;
	SceneObjects m_objects;

	typedef std::vector<ILightSourcePtr> LightSources;
	LightSources m_lightSources;

	// Цвет заднего фона сцены
	Vector4f m_backdropColor;
};