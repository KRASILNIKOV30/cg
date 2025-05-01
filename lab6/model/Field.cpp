#include "Field.h"

Field::Field()
	: m_grid(FIELD_SCHEME)
{
}

BlockType Field::GetBlock(const Point& pos) const
{
	return IsValidPosition(pos)
		? static_cast<BlockType>(m_grid[pos.y][pos.x])
		: ARMOR;
}

bool Field::CanTankPass(const Point& pos) const
{
	return GetBlock(pos) == EARTH;
}

bool Field::CanProjectilePass(const Point& pos) const
{
	return GetBlock(pos) != ARMOR;
}

bool Field::DestroyBlock(const Point& pos)
{
	if (!IsValidPosition(pos) || m_grid[pos.y][pos.x] != BRICK)
	{
		return false;
	}

	m_grid[pos.y][pos.x] = EARTH;
	return true;
}

bool Field::IsValidPosition(const Point& pos)
{
	return pos.x >= 0 && pos.x < FIELD_WIDTH &&
		pos.y >= 0 && pos.y < FIELD_HEIGHT;
}