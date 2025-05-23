#pragma once
#include "IShader.h"
#include "../material/SimpleMaterial.h"
#include "ShadeContext.h"
#include "../scene/Scene.h"

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
			const auto surfacePoint = shadeContext.GetSurfacePoint();
			Vector3d lightDirection = light.GetDirectionFromPoint(surfacePoint);

			const auto shadowFactor = GetShadow(surfacePoint, light, scene);
			const double lightIntensity = light.GetIntensityInDirection(-lightDirection) * shadowFactor;

			Vector3d const& n = shadeContext.GetSurfaceNormal();
			const auto normLightDir = Normalize(lightDirection);

			const double nDotL = Max(Dot(n, normLightDir), 0.0);
			const Vector4f diffuse = static_cast<float>(nDotL * lightIntensity) * light.GetDiffuseIntensity() * m_material.GetDiffuseColor();
			shadedColor += diffuse;

			const Vector4f ambient = light.GetAmbientIntensity() * m_material.GetAmbientColor();
			shadedColor += ambient;

			Vector3d reflectedLightVector = Normalize(2 * nDotL * n - normLightDir);
			const double nDotH = Max(Dot(n, reflectedLightVector), 0.0);
			const auto shiness = m_material.GetShiness();
			const Vector4f specular = static_cast<float>(std::pow(nDotH, shiness) * lightIntensity) * light.GetSpecularIntensity() * m_material.GetSpecularColor();
			shadedColor += specular;
		}

		return shadedColor;
	}

private:
	[[nodiscard]] static double GetShadow(Vector3d const& surfacePoint, ILightSource const& light, Scene const& scene)
	{
		constexpr int shadowSamples = 100;
		int reached = 0;
		Intersection intersection;
		SceneObject const* sceneObj;

		for (int i = 0; i <= shadowSamples; ++i)
		{
			const auto lightDir = light.GetRandomDirectionFromPoint(surfacePoint);
			Ray shadowRay{ surfacePoint, lightDir };
			if (!scene.GetFirstHit(shadowRay, intersection, &sceneObj))
			{
				++reached;
			}
		}

		return static_cast<double>(reached) / shadowSamples;
	}

private:
	SimpleMaterial m_material;
};