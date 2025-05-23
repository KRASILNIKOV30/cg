#pragma once
#include "IShader.h"
#include "../material/SimpleMaterial.h"
#include "ShadeContext.h"
#include "../scene/Scene.h"

#include <iostream>

class PhongLightShader final : public IShader
{
public:
	explicit PhongLightShader(SimpleMaterial const& material)
		: m_material(material)
	{
	}

	void SetMaterial(SimpleMaterial const& material)
	{
		m_material = material;
	}

	[[nodiscard]] Vector4f Shade(ShadeContext const& shadeContext) const override
	{
		Scene const& scene = shadeContext.GetScene();
		Vector4f shadedColor;

		const size_t numLights = scene.GetLightsCount();

		// Пробегаемся по всем источникам света в сцене
		for (size_t i = 0; i < numLights; ++i)
		{
			ILightSource const& light = scene.GetLight(i);
			Vector3d lightDirection = light.GetDirectionFromPoint(shadeContext.GetSurfacePoint());
			const double lightIntensity = light.GetIntensityInDirection(-lightDirection);
			Vector3d const& n = shadeContext.GetSurfaceNormal();

			const double nDotL = Max(Dot(n, Normalize(lightDirection)), 0.0);
			const Vector4f diffuse = static_cast<float>(nDotL * lightIntensity) * light.GetDiffuseIntensity() * m_material.GetDiffuseColor();
			shadedColor += diffuse;

			const Vector4f ambient = light.GetAmbientIntensity() * m_material.GetAmbientColor();
			shadedColor += ambient;

			Vector3d reflectedLightVector = Normalize(2 * nDotL * n - Normalize(lightDirection));
			const double nDotH = Max(Dot(n, reflectedLightVector), 0.0);
			const auto shiness = m_material.GetShiness();
			const Vector4f specular = static_cast<float>(std::pow(nDotH, shiness) * lightIntensity) * light.GetSpecularIntensity() * m_material.GetSpecularColor();
			shadedColor += specular;
		}

		return shadedColor;
	}

private:
	SimpleMaterial m_material;
};