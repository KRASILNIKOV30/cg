#pragma once
#include "../Vector3.h"

/*
Класс "ограничивающий блок"
*/
class BoundingBox
{
public:
	BoundingBox();

	// Создаем блок по двум точкам, задающим минимальные
	// и максимальные координаты
	BoundingBox(Vector3f const& minCoord, Vector3f const& maxCoord);

	// Сообщаем о том, является ли блок пустым
	[[nodiscard]] bool IsEmpty() const;

	// Возвращаем результат объединения двух ограничивающих блоков
	[[nodiscard]] BoundingBox Union(BoundingBox const& other) const;

	// Возвращаем минимальные и максимальные координаты
	[[nodiscard]] Vector3f const& GetMinCoord() const;
	[[nodiscard]] Vector3f const& GetMaxCoord() const;

	// Возвращаем размер блока: (min - max) 
	[[nodiscard]] Vector3f GetSize() const;

	// Возвращаем координаты центра блока
	[[nodiscard]] Vector3f GetCenter() const;

private:
	bool m_isEmpty;
	Vector3f m_minCoord;
	Vector3f m_maxCoord;
};