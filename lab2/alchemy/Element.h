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
	{ ElType::FIRE, "Огонь", "fire.jpg" },
	{ ElType::WATER, "Вода", "water.jpeg" },
	{ ElType::GROUND, "Земля", "ground.jpeg" },
	{ ElType::AIR, "Воздух", "air.jpg" },
	{ ElType::LAVA, "Лава", "lava.jpeg", std::pair{ ElType::GROUND, ElType::FIRE } },
};
