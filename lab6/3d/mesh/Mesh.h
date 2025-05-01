#pragma once
#include "../boundingBox/BoundingBox.h"
#include <GL/gl.h>

class Mesh
{
public:
	Mesh(
		unsigned int vertexBufferOffset, // смещение в буфере вершин
		unsigned int indexBufferOffset, // смещение в буфере индексов
		unsigned vertexCount, // количество вершин
		unsigned indexCount, // количество индексов
		bool hasTextureCoords, // наличие текстурных координат
		BoundingBox const& boundingBox, // ограничивающий блок
		GLenum primitiveType, // тип примитивов сетки
		GLenum indexType // тип индексов сетки
		);

	Mesh(Mesh const& mesh) = delete;
	Mesh& operator=(Mesh const& mesh) = delete;

	// Есть ли в сетке текстурные координаты?
	[[nodiscard]] bool HasTextureCoords() const;

	// Возвращаем смещение относительно начала буфера вершин
	[[nodiscard]] unsigned int GetVertexBufferOffset() const;

	// Возвращаем смещение относительно начала буфера индексов
	[[nodiscard]] unsigned int GetIndexBufferOffset() const;

	// Возвращаем количество индексов
	[[nodiscard]] unsigned int GetIndexCount() const;

	// Возвращаем количество вершин
	[[nodiscard]] unsigned int GetVertexCount() const;

	// Возвращаем ограничивающий блок
	[[nodiscard]] BoundingBox const& GetBoundingBox() const;

	// Возвращаем тип примитивов (GL_TRIANGLES, GL_TRIANGLE_STRIP и т.п.)
	[[nodiscard]] GLenum GetPrimitiveType() const;

	// Возвращаем тип данных для хранения индексов (GL_UNSIGNED_SHORT и т.п.)
	[[nodiscard]] GLenum GetIndexType() const;

private:
	unsigned int m_vertexBufferOffset;
	unsigned int m_indexBufferOffset;
	unsigned int m_vertexCount;
	unsigned int m_indexCount;
	bool m_hasTexture;
	BoundingBox const m_boundingBox;
	GLenum m_primitiveType;
	GLenum m_indexType;
};
