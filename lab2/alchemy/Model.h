#pragma once
#include "Element.h"
#include "../../lib/signals/SignallingValue.h"
#include <ranges>
#include <cmath>
#include <unordered_map>
#include <vector>

struct Point
{
	int x;
	int y;

	Point& operator+=(Point const& point)
	{
		x += point.x;
		y += point.y;
		return *this;
	}
};

struct Size
{
	int width;
	int height;
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

constexpr int ACTION_DISTANCE = 50;

class Model
{
public:
	using UpdateSignal = EmptySignal;
	using UpdateSlot = UpdateSignal::slot_type;

	explicit Model(std::vector<Element> const& elements)
	{
		for (const auto& element : elements)
		{
			const auto [type, name, img, combination] = element;
			m_elements.emplace(type, ElementModel{ type, name, img, !combination.has_value() });
			m_combinations[combination->first].emplace(combination->second, type);
		}
	}

	[[nodiscard]] ScopedConnection DoOnUpdate(UpdateSlot const& slot)
	{
		return m_updateSignal.connect(slot);
	}

	[[nodiscard]] ScopedConnection DoOnCanvasUpdate(UpdateSlot const& slot)
	{
		return m_canvasUpdateSignal.connect(slot);
	}

	void ForEachCanvasElement(std::function<void(CanvasElement const&)> const& callback)
	{
		for (const auto& element : std::views::values(m_canvasElements))
		{
			callback(element);
		}
	}

	void ForEachElement(std::function<void(ElementModel const&)> const& callback)
	{
		for (const auto& element : std::views::values(m_elements))
		{
			if (element.open)
			{
				callback(element);
			}
		}
	}

	void Move(size_t const id, Point const& delta)
	{
		m_canvasElements.at(id).position += delta;
		m_canvasUpdateSignal();
	}

	void Drop(size_t const id)
	{
		CheckCombinations(id);
		CheckTrash(id);
		m_canvasUpdateSignal();
		m_updateSignal();
	}

	void CreateElement(ElType const type)
	{
		AddElement(type, Point{ 100, 100 });
	}

private:
	void CheckCombinations(size_t const targetId)
	{
		const auto nearbyElementIds = GetNearbyElementIds(targetId);
		for (const auto id : nearbyElementIds)
		{
			TryCreateNewElement(targetId, id);
		}
	}

	void CheckTrash(size_t const id)
	{
		if (ArePointsNearby(m_trashPosition, m_canvasElements.at(id).position))
		{
			m_canvasElements.erase(id);
		}
	}

	void TryCreateNewElement(size_t const firstId, size_t const secondId)
	{
		const auto firstEl = m_canvasElements.at(firstId);
		const auto secondEl = m_canvasElements.at(secondId);
		const auto typeA = firstEl.type;
		const auto typeB = secondEl.type;
		if (m_combinations.at(typeA).contains(typeB))
		{
			m_canvasElements.erase(firstId);
			m_canvasElements.erase(secondId);
			const auto newElementPos = Point{
				(firstEl.position.x + secondEl.position.x) / 2,
				(firstEl.position.y + secondEl.position.y) / 2
			};
			const auto newElementType = m_combinations.at(typeA).at(typeB);
			m_elements.at(newElementType).open = true;
			AddElement(newElementType, newElementPos);
		}
	}

	void AddElement(ElType type, Point const& position)
	{
		const auto id = m_id++;
		const auto model = m_elements.at(type);
		m_canvasElements.emplace(id, CanvasElement{
			.id = id,
			.type = type,
			.name = model.name,
			.img = model.img,
			.position = position,
		});
	}

	std::vector<size_t> GetNearbyElementIds(size_t const targetId)
	{
		const auto target = m_canvasElements.at(targetId);
		std::vector<size_t> result;
		for (const auto& [id, element] : m_canvasElements)
		{
			if (id != targetId && ArePointsNearby(element.position, target.position))
			{
				result.push_back(id);
			}
		}

		return result;
	}

	static bool ArePointsNearby(Point const& p1, Point const& p2)
	{
		const auto distance = std::hypot(p1.x - p2.x, p1.y - p2.y);
		return distance <= ACTION_DISTANCE;
	}

private:
	UpdateSignal m_updateSignal;
	UpdateSignal m_canvasUpdateSignal;
	std::unordered_map<ElType, ElementModel> m_elements{};
	std::unordered_map<size_t, CanvasElement> m_canvasElements{};
	std::unordered_map<ElType, std::unordered_map<ElType, ElType>> m_combinations{};
	Point m_trashPosition = { 300, 600 };
	size_t m_id = 0;
};
