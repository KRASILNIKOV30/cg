#include "Model.h"
#include "../material/ModelMaterial.h"
#include "../textureMap/TextureMap.h"
#include "../mesh/Mesh.h"

bool Model::CompareTextureNames::operator()(std::string const& textureName1, std::string const& textureName2) const
{
	// Переводим имя первой текстуры к нижнему регистру
	std::string name1LowerCase(textureName1);
	std::transform(
		name1LowerCase.begin(),
		name1LowerCase.end(),
		name1LowerCase.begin(),
		tolower);

	// Переводим имя второй текстуры к нижнему регистру
	std::string name2LowerCase(textureName2);
	std::transform(
		name2LowerCase.begin(),
		name2LowerCase.end(),
		name2LowerCase.begin(),
		tolower);

	// Возвращаем результат сравнения имен в нижнем регистре
	return name1LowerCase < name2LowerCase;
}

Model::Model()
	: m_boundingBoxMustBeUpdated(true)
{
}

ModelMaterial& Model::AddMaterial()
{
	ModelMaterialPtr pMaterial(new ModelMaterial());
	m_materials.push_back(pMaterial);
	return *pMaterial;
}

size_t Model::GetMeterialCount() const
{
	return m_materials.size();
}

ModelMaterial const& Model::GetMaterial(size_t index) const
{
	return *m_materials.at(index);
}

ModelMaterial& Model::GetMaterial(size_t index)
{
	return *m_materials.at(index);
}

Texture2D& Model::AddTextureImage(std::string const& name)
{
	// Ищем, есть ли текстура с таким именем
	auto it = m_textures.find(name);
	if (it != m_textures.end())
	{
		// Если есть, возвращаем ссылку на существующую
		return *it->second;
	}

	// В противном случае создаем новую текстуру
	Texture2DPtr pTexture(new Texture2D());
	// и добавляем ее в контейнер текстур,
	m_textures.insert(CTextures::value_type(name, pTexture));
	// а ее имя в вектор имен
	m_textureNames.push_back(name);

	// возвращаем ссылку на добавленную текстуру
	return *pTexture;
}

bool Model::HasTexture(std::string const& name) const
{
	const auto it = m_textures.find(name);
	return it != m_textures.end();
}

size_t Model::GetTexturesCount() const
{
	return m_textures.size();
}

std::string Model::GetTextureName(size_t index) const
{
	return m_textureNames.at(index);
}

// Возвращаем текстурный объект по его имени
Texture2D& Model::GetTextureByName(std::string const& name)
{
	auto it = m_textures.find(name);
	if (it == m_textures.end())
	{
		throw std::logic_error("Texture with the specified name does not exist");
	}
	return *it->second;
}

// Возвращаем текстурный объект по его имени
Texture2D const& Model::GetTextureByName(std::string const& name) const
{
	const auto it = m_textures.find(name);
	if (it == m_textures.end())
	{
		throw std::logic_error("Texture with the specified name does not exist");
	}
	return *it->second;
}

// Возвращаем текстурный объект по его индексу
Texture2D& Model::GetTexture(size_t index)
{
	std::string textureName = m_textureNames.at(index);
	return GetTextureByName(textureName);
}

// Возвращаем текстурный объект по его индексу
Texture2D const& Model::GetTexture(size_t index) const
{
	const std::string textureName = m_textureNames.at(index);
	return GetTextureByName(textureName);
}

Mesh& Model::AddMesh(unsigned int vertexBufferOffset,
	unsigned int indexBufferOffset,
	unsigned vertexCount,
	unsigned indexCount,
	bool hasTexture,
	BoundingBox const& boundingBox,
	GLenum primitiveType,
	GLenum indexType
	)
{
	MeshPtr pMesh(new Mesh(
		vertexBufferOffset,
		indexBufferOffset,
		vertexCount,
		indexCount,
		hasTexture,
		boundingBox,
		primitiveType,
		indexType
		));
	m_meshes.push_back(pMesh);

	// При добавлении полигональной сетки старый bounding box
	// может оказаться неактуальным, поэтому делаем пометку о необходимости
	// его обновления
	m_boundingBoxMustBeUpdated = true;

	return *pMesh;
}

size_t Model::GetMeshCount() const
{
	return m_meshes.size();
}

Mesh const& Model::GetMesh(size_t index) const
{
	return *m_meshes.at(index);
}

Mesh& Model::GetMesh(size_t index)
{
	return *m_meshes.at(index);
}

CVertexBuffer& Model::GetVertexBuffer()
{
	return m_vertexBuffer;
}

CVertexBuffer const& Model::GetVertexBuffer() const
{
	return m_vertexBuffer;
}

CIndexBuffer& Model::GetIndexBuffer()
{
	return m_indexBuffer;
}

CIndexBuffer const& Model::GetIndexBuffer() const
{
	return m_indexBuffer;
}

BoundingBox Model::GetBoundingBox() const
{
	if (m_boundingBoxMustBeUpdated)
	{
		// Ограничивающий блок модели равен объединению ограничивающих блоков
		// всех сеток, входящих в состав модели
		BoundingBox box;
		for (const auto& m_mesh : m_meshes)
		{
			box = box.Union(m_mesh->GetBoundingBox());
		}
		m_boundingBox = box;

		// ограничивающий блок теперь актуален
		m_boundingBoxMustBeUpdated = false;
	}

	return m_boundingBox;
}