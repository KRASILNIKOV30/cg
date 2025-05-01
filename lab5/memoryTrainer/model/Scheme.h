#pragma once
#include <array>
#include <string>

enum class State
{
	FacedFront,
	Teasing,
	FacedBack,
	Dead,
};

struct Tile
{
	std::string name;
	State state = State::FacedFront;
};

constexpr int FIELD_WIDTH = 5;
constexpr int FIELD_HEIGHT = 4;

using Tiles = std::array<std::array<Tile, FIELD_WIDTH>, FIELD_HEIGHT>;

inline Tiles tilesScheme = {
	std::array{ Tile{ "apple" }, Tile{ "bed" }, Tile{ "cake" }, Tile{ "carrot" }, Tile{ "chest" } },
	std::array{ Tile{ "diamond" }, Tile{ "iron" }, Tile{ "kelp" }, Tile{ "leaf" }, Tile{ "music" } },
	std::array{ Tile{ "apple" }, Tile{ "bed" }, Tile{ "cake" }, Tile{ "carrot" }, Tile{ "chest" } },
	std::array{ Tile{ "diamond" }, Tile{ "iron" }, Tile{ "kelp" }, Tile{ "leaf" }, Tile{ "music" } }
};