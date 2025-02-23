#pragma once
#include "Element.h"

#include <unordered_map>
#include <vector>

struct Point
{
	int x;
	int y;
};

struct ElementModel
{
	ElType type;
	std::string name;
	std::string img;
	bool open;
};

struct CanvasElement
{
	size_t id;
	ElType type;
	std::string name;
	std::string img;
	Point position;
};

class Model
{
public:
	explicit Model(std::vector<Element> const& elements)
	{
		for (const auto& element : elements)
		{
			const auto [type, name, img, combination] = element;
			m_elements.emplace(type, ElementModel{ type, name, img, false });
			m_combinations[combination->first].emplace(combination->second, type);
		}
	}

private:
	std::unordered_map<ElType, ElementModel> m_elements{};
	std::unordered_map<size_t, CanvasElement> m_canvasElements{};
	std::unordered_map<ElType, std::unordered_map<ElType, ElType>> m_combinations{};
	Point m_trashPosition = { 300, 600 };
};