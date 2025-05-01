#pragma once
#include "Point.h"
#include "FieldScheme.h"

class Field
{
public:
	Field();

	[[nodiscard]] BlockType GetBlock(const Point& pos) const;
	[[nodiscard]] bool CanTankPass(const Point& pos) const;
	[[nodiscard]] bool CanProjectilePass(const Point& pos) const;
	bool DestroyBlock(const Point& pos);
	[[nodiscard]] static bool IsValidPosition(const Point& pos);

private:
	FieldType m_grid;
};