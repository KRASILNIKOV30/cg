#pragma once
#include "../geometry/vector/Vector4.h"

class ShadeContext;

/*
Интерфейс "шейдер", выполняющий расчет цвета объекта в заданной точке с использованием
некоторой модели освещения
*/
class IShader
{
public:
	// Выполняет вычисление цвета с использованием указанного контекста закрашивания
	[[nodiscard]] virtual Vector4f Shade(ShadeContext const& shadeContext) const = 0;

	virtual ~IShader() = default;
};