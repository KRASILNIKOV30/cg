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
	STEAM,
	MUD,
	ROCK,
	TREE,
	PLANT,
	ANIMAL,
	HUMAN,
	METAL,
	OIL,
	ELECTRICITY,
	GAS,
	CARBONATE,
	MINERAL,
	MERCURY,
	SAND,
	GLASS,
	LIGHT,
	DARK,
	POISON
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
	{ ElType::WATER, "Water", "water.jpg" },
	{ ElType::GROUND, "Ground", "ground.jpg" },
	{ ElType::AIR, "Air", "air.jpg" },
	{ ElType::LAVA, "Lava", "lava.jpg", std::pair{ ElType::GROUND, ElType::FIRE } },
	{ ElType::STEAM, "Steam", "steam.jpg", std::pair{ ElType::WATER, ElType::FIRE } },
	{ ElType::MUD, "Mud", "mud.jpg", std::pair{ ElType::WATER, ElType::GROUND } },
	{ ElType::ROCK, "Rock", "rock.jpg", std::pair{ ElType::GROUND, ElType::GROUND } },
	{ ElType::TREE, "Tree", "tree.jpg", std::pair{ ElType::PLANT, ElType::GROUND } },
	{ ElType::PLANT, "Plant", "plant.jpg", std::pair{ ElType::MUD, ElType::STEAM } },
	{ ElType::ANIMAL, "Animal", "animal.jpg", std::pair{ ElType::TREE, ElType::WATER } },
	{ ElType::HUMAN, "Human", "human.jpg", std::pair{ ElType::ANIMAL, ElType::AIR } },
	{ ElType::METAL, "Metal", "metal.jpg", std::pair{ ElType::ROCK, ElType::FIRE } },
	{ ElType::OIL, "Oil", "oil.jpg", std::pair{ ElType::PLANT, ElType::ROCK } },
	{ ElType::ELECTRICITY, "Electricity", "electricity.jpg", std::pair{ ElType::METAL, ElType::HUMAN } },
	{ ElType::GAS, "Gas", "gas.jpg", std::pair{ ElType::WATER, ElType::AIR } },
	{ ElType::CARBONATE, "Carbonate", "carbonate.jpg", std::pair{ ElType::WATER, ElType::METAL } },
	{ ElType::MINERAL, "Mineral", "mineral.jpg", std::pair{ ElType::WATER, ElType::ROCK } },
	{ ElType::MERCURY, "Mercury", "mercury.jpg", std::pair{ ElType::METAL, ElType::WATER } },
	{ ElType::SAND, "Sand", "sand.jpg", std::pair{ ElType::ROCK, ElType::GROUND } },
	{ ElType::GLASS, "Glass", "glass.jpg", std::pair{ ElType::SAND, ElType::FIRE } },
	{ ElType::LIGHT, "Light", "light.jpg", std::pair{ ElType::FIRE, ElType::AIR } },
	{ ElType::DARK, "Dark", "dark.jpg", std::pair{ ElType::AIR, ElType::GROUND } },
	{ ElType::POISON, "Poison", "poison.jpg", std::pair{ ElType::PLANT, ElType::ANIMAL } }
};

