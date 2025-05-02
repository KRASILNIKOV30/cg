#pragma once

#include "Material.h"
#include "lab6/3d/textureMap/TextureMap.h"

class ModelMaterial
{
public:
	ModelMaterial();

	ModelMaterial(ModelMaterial const& other) = delete;
	ModelMaterial& operator=(ModelMaterial const& other) = delete;

	// Добавить текстурную карту №1 к материалу
	TextureMap& AddTextureMap1(GLuint texture);

	// Связана ли с материалом текстурная карта №1?
	bool HasTextureMap1() const;

	// Получить текстурную карту №1
	TextureMap const& GetTextureMap1() const;
	TextureMap& GetTextureMap1();

	// Получить материал OpenGL
	Material& GetMaterial();
	Material const& GetMaterial() const;

	// Материал видим с лицевой и нелицевой грани?
	bool IsTwoSided() const;
	void SetTwoSided(bool value);

private:
	TextureMap m_textureMap1;
	Material m_material;
	bool m_twoSided;
};