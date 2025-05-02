#include "ModelLoader.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include "../material/Material.h"
#include "../../../lib/texture/TextureLoader.h"
#include "../material/ModelMaterial.h"
#include "../textureMap/TextureMap.h"
#include "lab6/3d/Vertex.h"

#include <cfloat>

ModelLoader::ModelLoader()
	: m_vertexBufferUsage(GL_STATIC_DRAW)
	  , m_indexBufferUsage(GL_STATIC_DRAW)
	  , m_ignoreMissingTextures(true)
{
}

void ModelLoader::IgnoreMissingTextures(bool ignore)
{
	m_ignoreMissingTextures = ignore;
}

void ModelLoader::SetVertexBufferUsage(GLenum usage)
{
	m_vertexBufferUsage = usage;
}

void ModelLoader::SetIndexBufferUsage(GLenum usage)
{
	m_indexBufferUsage = usage;
}

void ModelLoader::LoadObjFile(const std::string& filename, Model& model) const
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	// Определяем базовую папку для поиска текстур
	size_t slashPos = filename.find_last_of("/\\");
	std::string baseFolder = (slashPos == std::string::npos)
		? ""
		: filename.substr(0, slashPos + 1);

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), baseFolder.c_str()))
	{
		throw std::runtime_error(warn + err);
	}

	if (!warn.empty())
	{
		std::cout << "Warning: " << warn << std::endl;
	}

	ProcessMaterials(attrib, materials, model, baseFolder);
	ProcessShapes(attrib, shapes, materials, model);
}

void ModelLoader::ProcessMaterials(
	const tinyobj::attrib_t& attrib,
	const std::vector<tinyobj::material_t>& materials,
	Model& model,
	const std::string& baseFolder) const
{
	for (const auto& mat : materials)
	{
		ModelMaterial& material = model.AddMaterial();
		Material& matInfo = material.GetMaterial();

		matInfo.SetAmbient(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
		matInfo.SetDiffuse(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
		matInfo.SetSpecular(mat.specular[0], mat.specular[1], mat.specular[2]);
		matInfo.SetShininess(mat.shininess);
		material.SetTwoSided(false); // По умолчанию

		// Загружаем диффузную текстуру
		if (!mat.diffuse_texname.empty())
		{
			try
			{
				GLuint texture = LoadTexture(mat.diffuse_texname, model, baseFolder);
				TextureMap& textureMap = material.AddTextureMap1(texture);
				textureMap.SetOffset(0.0f, 0.0f);
				textureMap.SetScale(1.0f, 1.0f);
				textureMap.SetRotation(0.0f);
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

	// Добавляем дефолтный материал, если в файле нет материалов
	if (materials.empty())
	{
		ModelMaterial& material = model.AddMaterial();
		Material& matInfo = material.GetMaterial();
		matInfo.SetAmbient(0.2f, 0.2f, 0.2f);
		matInfo.SetDiffuse(0.8f, 0.8f, 0.8f);
		matInfo.SetSpecular(0.0f, 0.0f, 0.0f);
		matInfo.SetShininess(1.0f);
	}
}

GLuint ModelLoader::LoadTexture(
	const std::string& name,
	Model& model,
	const std::string& baseFolder) const
{
	Texture2D& texture = model.AddTextureImage(name);
	if (!texture)
	{
		TextureLoader loader;
		std::string texturePath = baseFolder + name;
		texture.Attach(loader.LoadTexture2D(texturePath));
	}
	return texture;
}

void ModelLoader::ProcessShapes(
	const tinyobj::attrib_t& attrib,
	const std::vector<tinyobj::shape_t>& shapes,
	const std::vector<tinyobj::material_t>& materials,
	Model& model) const
{
	std::vector<unsigned char> vertexBufferData;
	std::vector<unsigned char> indexBufferData;

	// Для каждого меша создаем отдельный объект Mesh в модели
	for (const auto& shape : shapes)
	{
		// Проверяем наличие текстурных координат и нормалей
		bool hasTexCoords = false;
		bool hasNormals = false;
		for (const auto& index : shape.mesh.indices)
		{
			if (index.texcoord_index >= 0)
				hasTexCoords = true;
			if (index.normal_index >= 0)
				hasNormals = true;
		}

		// Смещения в буферах
		const size_t vertexBufferOffset = vertexBufferData.size();
		const size_t indexBufferOffset = indexBufferData.size();

		// Для хранения уникальных вершин и их индексов
		std::unordered_map<std::string, unsigned> vertexIndices;
		unsigned nextIndex = 0;

		// Для хранения граней, сгруппированных по материалам
		std::vector<MeshFace> faces;
		std::vector<std::vector<unsigned>> materialFaces(materials.size() + 1); // +1 для дефолтного материала

		// Для вычисления bounding box
		float minBounds[3] = { FLT_MAX, FLT_MAX, FLT_MAX };
		float maxBounds[3] = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

		// Обрабатываем все грани формы
		size_t index_offset = 0;
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
		{
			int fv = shape.mesh.num_face_vertices[f];
			if (fv != 3)
			{
				index_offset += fv;
				continue; // Пропускаем не треугольные грани
			}

			MeshFace face;
			face.materialIndex = shape.mesh.material_ids[f];
			if (face.materialIndex < -1 || face.materialIndex >= static_cast<int>(materials.size()))
			{
				face.materialIndex = -1; // Используем дефолтный материал
			}

			// Обрабатываем 3 вершины грани
			for (size_t v = 0; v < 3; v++)
			{
				tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

				// Создаем ключ для вершины
				std::string vertexKey = std::to_string(idx.vertex_index) + "_" +
					(idx.normal_index >= 0 ? std::to_string(idx.normal_index) : "x") + "_" +
					(idx.texcoord_index >= 0 ? std::to_string(idx.texcoord_index) : "x");

				// Если вершина уже встречалась, используем ее индекс
				if (vertexIndices.count(vertexKey))
				{
					face.vertices[v] = vertexIndices[vertexKey];
					continue;
				}

				// Добавляем новую вершину
				vertexIndices[vertexKey] = nextIndex++;
				face.vertices[v] = vertexIndices[vertexKey];

				// Позиция
				float x = attrib.vertices[3 * idx.vertex_index + 0];
				float y = attrib.vertices[3 * idx.vertex_index + 1];
				float z = attrib.vertices[3 * idx.vertex_index + 2];

				// Обновляем bounding box
				minBounds[0] = std::min(minBounds[0], x);
				minBounds[1] = std::min(minBounds[1], y);
				minBounds[2] = std::min(minBounds[2], z);
				maxBounds[0] = std::max(maxBounds[0], x);
				maxBounds[1] = std::max(maxBounds[1], y);
				maxBounds[2] = std::max(maxBounds[2], z);

				// Добавляем данные вершины в буфер
				if (hasTexCoords && hasNormals)
				{
					TexturedVertex vertex;
					vertex.position = { x, y, z };

					if (idx.normal_index >= 0)
					{
						vertex.normal = {
							attrib.normals[3 * idx.normal_index + 0],
							attrib.normals[3 * idx.normal_index + 1],
							attrib.normals[3 * idx.normal_index + 2]
						};
					}
					else
					{
						vertex.normal = { 0.0f, 0.0f, 0.0f };
					}

					if (idx.texcoord_index >= 0)
					{
						vertex.texCoord = {
							attrib.texcoords[2 * idx.texcoord_index + 0],
							1.0f - attrib.texcoords[2 * idx.texcoord_index + 1] // Инвертируем V
						};
					}
					else
					{
						vertex.texCoord = { 0.0f, 0.0f };
					}

					// Добавляем вершину в буфер
					const unsigned char* vertexData = reinterpret_cast<const unsigned char*>(&vertex);
					vertexBufferData.insert(vertexBufferData.end(), vertexData, vertexData + sizeof(vertex));
				}
				else if (hasNormals)
				{
					Vertex vertex;
					vertex.position = { x, y, z };

					if (idx.normal_index >= 0)
					{
						vertex.normal = {
							attrib.normals[3 * idx.normal_index + 0],
							attrib.normals[3 * idx.normal_index + 1],
							attrib.normals[3 * idx.normal_index + 2]
						};
					}
					else
					{
						vertex.normal = { 0.0f, 0.0f, 0.0f };
					}

					const unsigned char* vertexData = reinterpret_cast<const unsigned char*>(&vertex);
					vertexBufferData.insert(vertexBufferData.end(), vertexData, vertexData + sizeof(vertex));
				}
				else
				{
					// Простейший вариант - только позиции
					float vertexData[] = { x, y, z };
					vertexBufferData.insert(vertexBufferData.end(),
						reinterpret_cast<unsigned char*>(vertexData),
						reinterpret_cast<unsigned char*>(vertexData) + sizeof(vertexData));
				}
			}

			faces.push_back(face);
			materialFaces[face.materialIndex + 1].push_back(static_cast<unsigned>(faces.size() - 1));
			index_offset += 3;
		}

		// Определяем тип индексов
		GLenum indexType = GL_UNSIGNED_INT;
		if (vertexIndices.size() <= 0xFF + 1)
		{
			indexType = GL_UNSIGNED_BYTE;
		}
		else if (vertexIndices.size() <= 0xFFFF + 1)
		{
			indexType = GL_UNSIGNED_SHORT;
		}

		// Добавляем индексы в буфер
		for (const auto& face : faces)
		{
			for (unsigned v = 0; v < 3; ++v)
			{
				unsigned index = face.vertices[v];
				switch (indexType)
				{
				case GL_UNSIGNED_BYTE: {
					GLubyte byteIndex = static_cast<GLubyte>(index);
					const unsigned char* indexData = reinterpret_cast<const unsigned char*>(&byteIndex);
					indexBufferData.insert(indexBufferData.end(), indexData, indexData + sizeof(byteIndex));
					break;
				}
				case GL_UNSIGNED_SHORT: {
					GLushort shortIndex = static_cast<GLushort>(index);
					const unsigned char* indexData = reinterpret_cast<const unsigned char*>(&shortIndex);
					indexBufferData.insert(indexBufferData.end(), indexData, indexData + sizeof(shortIndex));
					break;
				}
				case GL_UNSIGNED_INT: {
					GLuint uintIndex = static_cast<GLuint>(index);
					const unsigned char* indexData = reinterpret_cast<const unsigned char*>(&uintIndex);
					indexBufferData.insert(indexBufferData.end(), indexData, indexData + sizeof(uintIndex));
					break;
				}
				}
			}
		}

		// Создаем bounding box
		BoundingBox meshBoundingBox(
			Vector3f(minBounds[0], minBounds[1], minBounds[2]),
			Vector3f(maxBounds[0], maxBounds[1], maxBounds[2]));

		// Добавляем меш в модель
		Mesh& mesh = model.AddMesh(
			static_cast<unsigned>(vertexBufferOffset),
			static_cast<unsigned>(indexBufferOffset),
			static_cast<unsigned>(vertexIndices.size()),
			static_cast<unsigned>(faces.size() * 3),
			hasTexCoords,
			meshBoundingBox,
			GL_TRIANGLES,
			indexType);

		// Добавляем подсетки для каждого материала
		unsigned submeshStartIndex = 0;
		for (size_t materialIndex = 0; materialIndex < materialFaces.size(); ++materialIndex)
		{
			const auto& subMeshFaces = materialFaces[materialIndex];
			if (!subMeshFaces.empty())
			{
				unsigned subMeshIndex = mesh.AddSubMesh(
					submeshStartIndex,
					static_cast<unsigned>(subMeshFaces.size() * 3));

				// Связываем материал с подсеткой
				mesh.SetMaterialSubMesh(
					static_cast<int>(materialIndex) - 1,
					subMeshIndex);

				submeshStartIndex += static_cast<unsigned>(subMeshFaces.size() * 3);
			}
		}
	}

	// Создаем буферы в OpenGL
	if (!vertexBufferData.empty())
	{
		model.GetVertexBuffer().Create();
		model.GetVertexBuffer().BufferData(
			vertexBufferData.size(),
			vertexBufferData.data(),
			m_vertexBufferUsage);
	}

	if (!indexBufferData.empty())
	{
		model.GetIndexBuffer().Create();
		model.GetIndexBuffer().BufferData(
			indexBufferData.size(),
			indexBufferData.data(),
			m_indexBufferUsage);
	}
}