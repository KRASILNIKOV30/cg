#include "Mesh.h"

Mesh::Mesh(
	unsigned int vertexBufferOffset,
	unsigned int indexBufferOffset,
	unsigned vertexCount,
	unsigned indexCount,
	bool hasTexture,
	BoundingBox const& boundingBox,
	GLenum primitiveType,
	GLenum indexType
	)
	: m_indexBufferOffset(indexBufferOffset)
	  , m_vertexBufferOffset(vertexBufferOffset)
	  , m_vertexCount(vertexCount)
	  , m_indexCount(indexCount)
	  , m_hasTexture(hasTexture)
	  , m_boundingBox(boundingBox)
	  , m_primitiveType(primitiveType)
	  , m_indexType(indexType)
{
}

bool Mesh::HasTextureCoords() const
{
	return m_hasTexture;
}

unsigned int Mesh::GetVertexBufferOffset() const
{
	return m_vertexBufferOffset;
}

unsigned int Mesh::GetIndexBufferOffset() const
{
	return m_indexBufferOffset;
}

unsigned int Mesh::GetIndexCount() const
{
	return m_indexCount;
}

BoundingBox const& Mesh::GetBoundingBox() const
{
	return m_boundingBox;
}

GLenum Mesh::GetPrimitiveType() const
{
	return m_primitiveType;
}

GLenum Mesh::GetIndexType() const
{
	return m_indexType;
}

unsigned int Mesh::GetVertexCount() const
{
	return m_vertexCount;
}