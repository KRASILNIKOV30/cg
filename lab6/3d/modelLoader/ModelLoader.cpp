#include "ModelLoader.h"
#include <iostream>
#include <unordered_map>
#include "../material/Material.h"
#include "../../../lib/texture/TextureLoader.h"
#include "lab6/3d/material/ModelMaterial.h"
#include "lab6/3d/textureMap/TextureMap.h"

#include <cfloat>

ModelLoader::ModelLoader()
	: m_vertexBufferUsage(GL_STATIC_DRAW),
	  m_indexBufferUsage(GL_STATIC_DRAW),
	  m_ignoreMissingTextures(true)
{
}

void ModelLoader::LoadObjFile(const std::string& filename, Model& model) const
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()))
	{
		throw std::runtime_error(warn + err);
	}

	if (!warn.empty())
	{
		std::cout << "Warning: " << warn << std::endl;
	}

	ProcessMaterials(attrib, materials, model);
	ProcessMesh(attrib, shapes, model);
}

void ModelLoader::ProcessMaterials(const tinyobj::attrib_t& attrib,
	const std::vector<tinyobj::material_t>& materials,
	Model& model) const
{
	for (const auto& mat : materials)
	{
		// Создаем материал и настраиваем его параметры
		ModelMaterial& material = model.AddMaterial();
		Material& matInfo = material.GetMaterial();

		matInfo.SetAmbient(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
		matInfo.SetDiffuse(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
		matInfo.SetSpecular(mat.specular[0], mat.specular[1], mat.specular[2]);
		matInfo.SetShininess(mat.shininess);

		if (!mat.diffuse_texname.empty())
		{
			try
			{
				Texture2D& texture = model.AddTextureImage(mat.diffuse_texname);
				if (!texture)
				{
					TextureLoader loader;
					texture.Attach(loader.LoadTexture2D(mat.diffuse_texname));
				}

				material.AddTextureMap1(texture);
				// Здесь должна быть ваша логика загрузки текстуры
				// texture = LoadTexture(mat.diffuse_texname);
				// material.AddTextureMap1(texture);
			}
			catch (const std::exception& e)
			{
				if (!m_ignoreMissingTextures)
				{
					throw;
				}
				std::cerr << "Failed to load texture: " << mat.diffuse_texname << std::endl;
			}
		}
	}
}

void ModelLoader::ProcessMesh(const tinyobj::attrib_t& attrib,
	const std::vector<tinyobj::shape_t>& shapes,
	Model& model) const
{
	std::vector<float> vertexBufferData;
	std::vector<unsigned int> indexBufferData;

	// Для вычисления bounding box
	float minBounds[3] = { FLT_MAX, FLT_MAX, FLT_MAX };
	float maxBounds[3] = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

	// Для хранения уникальных вершин и их индексов
	std::unordered_map<std::string, unsigned int> vertexIndices;
	unsigned int nextIndex = 0;

	bool hasTexCoords = false;
	bool hasNormals = false;

	// Проверяем наличие текстурных координат и нормалей
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			if (index.texcoord_index >= 0)
				hasTexCoords = true;
			if (index.normal_index >= 0)
				hasNormals = true;
		}
	}

	for (const auto& shape : shapes)
	{
		size_t index_offset = 0;

		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
		{
			int fv = shape.mesh.num_face_vertices[f];

			for (size_t v = 0; v < fv; v++)
			{
				tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

				// Создаем ключ для вершины
				std::string vertexKey = std::to_string(idx.vertex_index) + "_" +
					(idx.normal_index != -1 ? std::to_string(idx.normal_index) : "x") + "_" +
					(idx.texcoord_index != -1 ? std::to_string(idx.texcoord_index) : "x");

				// Если вершина уже встречалась, используем ее индекс
				if (vertexIndices.count(vertexKey))
				{
					indexBufferData.push_back(vertexIndices[vertexKey]);
					continue;
				}

				// Добавляем новую вершину
				vertexIndices[vertexKey] = nextIndex++;
				indexBufferData.push_back(vertexIndices[vertexKey]);

				// Позиция
				float x = attrib.vertices[3 * idx.vertex_index + 0];
				float y = attrib.vertices[3 * idx.vertex_index + 1];
				float z = attrib.vertices[3 * idx.vertex_index + 2];

				vertexBufferData.push_back(x);
				vertexBufferData.push_back(y);
				vertexBufferData.push_back(z);

				// Обновляем bounding box
				minBounds[0] = std::min(minBounds[0], x);
				minBounds[1] = std::min(minBounds[1], y);
				minBounds[2] = std::min(minBounds[2], z);
				maxBounds[0] = std::max(maxBounds[0], x);
				maxBounds[1] = std::max(maxBounds[1], y);
				maxBounds[2] = std::max(maxBounds[2], z);

				// Нормаль (если есть)
				if (hasNormals && idx.normal_index >= 0)
				{
					vertexBufferData.push_back(attrib.normals[3 * idx.normal_index + 0]);
					vertexBufferData.push_back(attrib.normals[3 * idx.normal_index + 1]);
					vertexBufferData.push_back(attrib.normals[3 * idx.normal_index + 2]);
				}

				// Текстурные координаты (если есть)
				if (hasTexCoords && idx.texcoord_index >= 0)
				{
					vertexBufferData.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
					vertexBufferData.push_back(1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]); // Инвертируем V координату
				}
			}
			index_offset += fv;
		}
	}

	// Создаем буферы в OpenGL
	model.GetVertexBuffer().Create();
	model.GetVertexBuffer().BufferData(
		vertexBufferData.size() * sizeof(float),
		vertexBufferData.data(),
		m_vertexBufferUsage);

	model.GetIndexBuffer().Create();
	model.GetIndexBuffer().BufferData(
		indexBufferData.size() * sizeof(unsigned int),
		indexBufferData.data(),
		m_indexBufferUsage);

	// Создаем bounding box
	BoundingBox meshBoundingBox(
		Vector3f(minBounds[0], minBounds[1], minBounds[2]),
		Vector3f(maxBounds[0], maxBounds[1], maxBounds[2])
		);

	// Добавляем меш в модель
	model.AddMesh(
		0, // vertexBufferOffset (у нас один буфер для всех мешей)
		0, // indexBufferOffset
		vertexIndices.size(), // vertexCount
		indexBufferData.size(), // indexCount
		hasTexCoords, // hasTexCoords
		meshBoundingBox,
		GL_TRIANGLES,
		GL_UNSIGNED_INT
		);
}