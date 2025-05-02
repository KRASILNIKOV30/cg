#include "Field.h"

Field::Field()
	: m_grid(FIELD_SCHEME)
{
}

BlockType Field::GetBlock(int x, int y) const
{
	return IsValidPosition(x, y)
		? static_cast<BlockType>(m_grid[y][x])
		: ARMOR;
}

bool Field::CanTankPass(int x, int y) const
{
	return GetBlock(x, y) == EARTH;
}

bool Field::CanProjectilePass(int x, int y) const
{
	return GetBlock(x, y) != ARMOR;
}

bool Field::DestroyBlock(int x, int y)
{
	if (!IsValidPosition(x, y) || m_grid[y][x] != BRICK)
	{
		return false;
	}

	m_grid[y][x] = EARTH;
	return true;
}

bool Field::IsValidPosition(int x, int y)
{
	return x >= 0 && x < FIELD_WIDTH &&
		y >= 0 && y < FIELD_HEIGHT;
}