#pragma once
#include <vector>
#include "HitInfo.h"

/*
Класс, хранящий информацию о точках пересечения луча с объектом сцены.
Первые 4 точки пересечения хранятся в кэше (обычный массив)
Остальные - в std::vector
*/
class Intersection
{
	enum
	{
		// Размер кэша точек пересечений
		HIT_CACHE_SIZE = 4,
	};

public:
	Intersection()
		: m_hitCount(0)
	{
	}

	/*
	Добавляем точку пересечения, возвращаем индекс добавленной точки пересечения
	*/
	size_t AddHit(HitInfo const& hit)
	{
		// Добавляем элемент в кэш, если он не заполнен
		if (m_hitCount < HIT_CACHE_SIZE)
		{
			m_hitCache[m_hitCount] = hit;
		}
		else // В противном случае - в vector
		{
			m_hits.push_back(hit);
		}

		return m_hitCount++;
	}

	/*
	Возвращает информацию о точке пересечения с указанным индексом
	*/
	[[nodiscard]] HitInfo const& GetHit(size_t index) const
	{
		assert(index < m_hitCount);

		// Возвращаем элемент либо из кэша
		if (index < HIT_CACHE_SIZE)
		{
			return m_hitCache[index];
		}

		return m_hits[index - HIT_CACHE_SIZE];
	}

	[[nodiscard]] size_t GetHitsCount() const
	{
		return m_hitCount;
	}

	// Очистка информации о точках столкновения
	void Clear()
	{
		if (m_hitCount > HIT_CACHE_SIZE)
		{
			m_hits.clear();
		}
		m_hitCount = 0;
	}

private:
	HitInfo m_hitCache[HIT_CACHE_SIZE]{};
	std::vector<HitInfo> m_hits{};
	size_t m_hitCount;
};