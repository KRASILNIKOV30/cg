#pragma once
#include "Scheme.h"

#include <cassert>

class Model
{
public:
	void Flip(const int x, const int y)
	{
		assert(x >=0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT);
		auto* tile = &m_tiles[x][y];
		tile->state = State::FacedFront;
		if (!m_activeTile)
		{
			m_activeTile = tile;
			return;
		}

		if (m_activeTile->name == tile->name)
		{
			m_activeTile->state = State::Dead;
			tile->state = State::Dead;
		}
		else
		{
			m_activeTile->state = State::Teasing;
			tile->state = State::Teasing;
		}

		m_activeTile = nullptr;
	}

	Tile GetTile(const int x, const int y)
	{
		assert(x >=0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT);
		return m_tiles[x][y];
	}

private:
	Tiles m_tiles = tilesScheme;
	Tile* m_activeTile = nullptr;
};