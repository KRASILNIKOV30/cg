#include "SceneObject.h"

#include <utility>

SceneObject::SceneObject(std::shared_ptr<IGeometryObject const> obj)
	: m_object(std::move(obj))
	  , m_shader(nullptr)
{
}

SceneObject::SceneObject(std::shared_ptr<IGeometryObject const> obj, std::shared_ptr<IShader const> shader)
	: m_object(std::move(obj))
	  , m_shader(std::move(shader))
{
}

// Дальше добавить класс Scene
