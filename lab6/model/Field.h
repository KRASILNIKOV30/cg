#pragma once
#include "Point.h"
#include "FieldScheme.h"

class Field
{
public:
	Field();

	[[nodiscard]] BlockType GetBlock(int x, int y) const;
	[[nodiscard]] bool CanTankPass(Point const& point) const;
	[[nodiscard]] bool CanProjectilePass(int x, int y) const;
	bool DestroyBlock(int x, int y);
	[[nodiscard]] static bool IsValidPosition(int x, int y);

private:
	FieldType m_grid;
};