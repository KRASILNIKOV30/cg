#pragma once
#include "../geometry/vector/Vector4.h"

/*
Простейший материал, для которого задан только лишь диффузный цвет
*/
class SimpleMaterial
{
public:
	explicit SimpleMaterial(Vector4f const& diffuseColor)
		: m_diffuseColor(diffuseColor)
	{
	}

	[[nodiscard]] Vector4f GetDiffuseColor() const
	{
		return m_diffuseColor;
	}

	void SetDiffuseColor(Vector4f const& diffuseColor)
	{
		m_diffuseColor = diffuseColor;
	}

private:
	Vector4f m_diffuseColor;
};