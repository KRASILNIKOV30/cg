#pragma once
#include <optional>
#include <string>
#include <vector>

enum class ElType
{
	FIRE,
	WATER,
	GROUND,
	AIR,
	LAVA,
};

struct Element
{
	ElType type;
	std::string name;
	std::string img;
	std::optional<std::pair<ElType, ElType>> combination = std::nullopt;
};

const std::vector<Element> elements = {
	{ ElType::FIRE, "Fire", "fire.jpg" },
	{ ElType::WATER, "Water", "water.jpeg" },
	{ ElType::GROUND, "Ground", "ground.jpeg" },
	{ ElType::AIR, "Air", "air.jpg" },
	{ ElType::LAVA, "Lava", "lava.jpeg", std::pair{ ElType::GROUND, ElType::FIRE } },
};
