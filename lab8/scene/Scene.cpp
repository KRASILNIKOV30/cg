#include "Scene.h"
#include "../geometry/intersection/Intersection.h"
#include "../geometry/IGeometryObject.h"
#include "sceneObject/SceneObject.h"
#include "../shader/IShader.h"
#include "../shader/ShadeContext.h"

/*
Задает цвет заднего фона, который будет возвращаться при трассировке лучей,
не пересекающихся ни с одним из объектов сцены
*/
void Scene::SetBackdropColor(Vector4f const& backdropColor)
{
	m_backdropColor = backdropColor;
}

/*
Добавляем источник света к сцене
*/
void Scene::AddLightSource(const ILightSourcePtr& pLightSource)
{
	m_lightSources.push_back(pLightSource);
}

/*
Возврат количества источников света в сцене
*/
size_t Scene::GetLightsCount() const
{
	return m_lightSources.size();
}

/*
Возвращаем ссылку на источник света с указанным индексом
*/
ILightSource const& Scene::GetLight(size_t index) const
{
	assert(index < m_lightSources.size());

	return *m_lightSources[index];
}

/*
Добавляем в сцену объект
*/
void Scene::AddObject(const SceneObjectPtr& pSceneObject)
{
	m_objects.push_back(pSceneObject);
}

Vector4f Scene::Shade(Ray const& ray) const
{
	Intersection bestIntersection;
	SceneObject const* pSceneObject = nullptr;

	// Находим первое столкновение луча со сценой
	if (GetFirstHit(ray, bestIntersection, &pSceneObject))
	{
		// Связан ли шейдер с найденным объектом сцены?
		if (pSceneObject->HasShader())
		{
			IShader const& shader = pSceneObject->GetShader();

			// Берем информацию о первом столкновении луча с объектом
			HitInfo const& hit = bestIntersection.GetHit(0);

			// Инициализируем контекст закрашивания для передачи его шейдеру
			// Контекст затенения хранит информацию о закрашиваемой точке, а также о сцене
			ShadeContext shadeContext(
				*this,
				hit.GetHitPoint(),
				hit.GetHitPointInObjectSpace(),
				hit.GetNormal(),
				ray.GetDirection()
				);

			// Шейдер, связанный с объектом, выполнит вычисление цвета
			return shader.Shade(shadeContext);
		}
	}

	// Точек пересечения данного луча с объектами сцены нет,
	// поэтому возвращаем цвет заднего фона
	return m_backdropColor;
}

bool Scene::GetFirstHit(Ray const& ray, Intersection& bestIntersection, SceneObject const** ppIntersectionObject) const
{
	// Очищаем информацию о точках столкновения
	bestIntersection.Clear();

	const size_t n = m_objects.size();

	Intersection intersection;

	// Пробегаем по всем объектам сцены
	for (size_t i = 0; i < n; ++i)
	{
		// Очищаем информацию о ранее найденных столкновениях
		intersection.Clear();

		SceneObject const& sceneObject = *m_objects[i];

		// Получаем геометрический объект, связанный с объектом сцены
		IGeometryObject const& geometryObject = sceneObject.GetGeometryObject();

		// Ищем точки пересечения луча с данным геометрическим обетом
		if (geometryObject.Hit(ray, intersection))
		{
			// Точки пересечения есть

			// Если это первый объект, с которым было найдено пересечение,
			// либо момент столкновения луча с данным объектом произошел раньше
			// (точка пересечения с текущим объектом находится ближе к точке испускания 
			// луча, чем с ранее обработанными объектами),
			// то за лучшую точку пересечения принимается найденная точка пересечения
			if (
				(bestIntersection.GetHitsCount() == 0) ||
				(intersection.GetHit(0).GetHitTime() < bestIntersection.GetHit(0).GetHitTime())
			)
			{
				bestIntersection = intersection;
				*ppIntersectionObject = &sceneObject;
			}
		}
	}

	// Возвращаем true, если было найдено хоть одно столкновение
	return bestIntersection.GetHitsCount() > 0;
}
