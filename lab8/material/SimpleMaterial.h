#pragma once
#include "../geometry/vector/Vector4.h"

class SimpleMaterial
{
public:
	explicit SimpleMaterial(
		Vector4f const& diffuseColor,
		Vector4f const& ambientColor,
		Vector4f const& specularColor,
		const double shiness
		)
		: m_diffuseColor(diffuseColor)
		  , m_ambientColor(ambientColor)
		  , m_specularColor(specularColor)
		  , m_shiness(shiness)
	{
	}

	[[nodiscard]] Vector4f GetDiffuseColor() const
	{
		return m_diffuseColor;
	}

	[[nodiscard]] Vector4f GetAmbientColor() const
	{
		return m_ambientColor;
	}

	[[nodiscard]] Vector4f GetSpecularColor() const
	{
		return m_specularColor;
	}

	[[nodiscard]] double GetShiness() const
	{
		return m_shiness;
	}

private:
	Vector4f m_diffuseColor;
	Vector4f m_ambientColor;
	Vector4f m_specularColor;
	double m_shiness;
};