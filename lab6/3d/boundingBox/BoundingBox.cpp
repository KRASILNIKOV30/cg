#include "BoundingBox.h"

#include <stdexcept>

BoundingBox::BoundingBox()
	: m_isEmpty(true)
{
}

BoundingBox::BoundingBox(Vector3f const& minCoord, Vector3f const& maxCoord)
	: m_minCoord(minCoord)
	  , m_maxCoord(maxCoord)
	  , m_isEmpty(false)
{
}

Vector3f const& BoundingBox::GetMinCoord() const
{
	if (m_isEmpty)
	{
		throw std::logic_error("Bounding box is empty");
	}
	return m_minCoord;
}

Vector3f const& BoundingBox::GetMaxCoord() const
{
	if (m_isEmpty)
	{
		throw std::logic_error("Bounding box is empty");
	}
	return m_maxCoord;
}

Vector3f BoundingBox::GetSize() const
{
	if (m_isEmpty)
	{
		return { 0, 0, 0 };
	}
	return m_maxCoord - m_minCoord;
}

Vector3f BoundingBox::GetCenter() const
{
	if (m_isEmpty)
	{
		throw std::logic_error("Bounding box is empty");
	}
	return (m_minCoord + m_maxCoord) * 0.5f;
}

BoundingBox BoundingBox::Union(BoundingBox const& other) const
{
	if (m_isEmpty && other.m_isEmpty) // оба блока пусты
	{
		return {};
	}

	if (m_isEmpty) // пустой только текущий
	{
		return other;
	}

	if (other.m_isEmpty) // текущий - непустой
	{
		return *this;
	}
	// объединяем два непустых блока
	using namespace std;
	return {
		Vector3f(
			min(m_minCoord.x, other.m_minCoord.x),
			min(m_minCoord.y, other.m_minCoord.y),
			min(m_minCoord.z, other.m_minCoord.z)
			),
		Vector3f(
			max(m_maxCoord.x, other.m_maxCoord.x),
			max(m_maxCoord.y, other.m_maxCoord.y),
			max(m_maxCoord.z, other.m_maxCoord.z)
			)
	};
}

bool BoundingBox::IsEmpty() const
{
	return m_isEmpty;
}