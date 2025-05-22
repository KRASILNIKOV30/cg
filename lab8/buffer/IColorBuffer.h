#pragma once
#include <cstdint>

/*
 Вспомогательный интержейс "Буфер цвета". Служит для абстрагирования
 класса RenderContext от деталей реализации буфера кадра,
 в котором строится изображение
 */
class IColorBuffer
{
public:
	virtual void SetPixelColor(unsigned x, unsigned y, uint32_t color) = 0;
	virtual ~IColorBuffer() = default;
};