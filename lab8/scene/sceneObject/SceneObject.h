#pragma once
#include "../../geometry/IGeometryObject.h"
#include "../../shader/IShader.h"
#include <memory>

/*
Объект сцены - геометрический объект, с которым связан некоторый шейдер,
выполняющий расчет цвета
*/
class SceneObject
{
public:
	/*
	Конструирует объект сцены, не имеющий шейдер
	*/
	explicit SceneObject(std::shared_ptr<IGeometryObject const> obj);

	/*
	Конструирует объект сцены, связывая с ним шейдер
	*/
	SceneObject(std::shared_ptr<IGeometryObject const> obj, std::shared_ptr<IShader const> shader);

	/*
	Возвращает ссылку на геометрический объект
	*/
	[[nodiscard]] IGeometryObject const& GetGeometryObject() const noexcept
	{
		return *m_object;
	}

	// Есть ли шейдер у объекта сцены?
	[[nodiscard]] bool HasShader() const noexcept
	{
		return m_shader != nullptr;
	}

	// Возвращает связанный шейдер
	[[nodiscard]] IShader const& GetShader() const noexcept
	{
		assert(HasShader());
		return *m_shader;
	}

private:
	std::shared_ptr<IGeometryObject const> m_object;
	std::shared_ptr<IShader const> m_shader;
};

using SceneObjectPtr = std::shared_ptr<SceneObject>;