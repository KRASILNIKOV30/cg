#pragma once
#include "../model/Model.h"

class TileView
{
public:
	explicit TileView(Tile* tile)
		: m_tile(tile)
	{
	}

	void Draw()
	{

	}

private:
	Tile* m_tile;
};