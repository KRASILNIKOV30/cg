#pragma once
#include "../../../lib/tinyobjloader/tiny_obj_loader.h"
#include <vector>
#include <GL/glew.h>
#include "../model/Model.h"

class ModelLoader
{
public:
	ModelLoader();
	void LoadObjFile(const std::string& filename, Model& model) const;
	void IgnoreMissingTextures(bool ignore);
	void SetVertexBufferUsage(GLenum usage);
	void SetIndexBufferUsage(GLenum usage);

private:
	struct MeshFace
	{
		unsigned vertices[3];
		int materialIndex;
	};

	void ProcessMaterials(
		const tinyobj::attrib_t& attrib,
		const std::vector<tinyobj::material_t>& materials,
		Model& model,
		const std::string& baseFolder) const;

	void ProcessShapes(
		const tinyobj::attrib_t& attrib,
		const std::vector<tinyobj::shape_t>& shapes,
		const std::vector<tinyobj::material_t>& materials,
		Model& model) const;

	GLuint LoadTexture(
		const std::string& name,
		Model& model,
		const std::string& baseFolder = "") const;

	GLenum m_vertexBufferUsage;
	GLenum m_indexBufferUsage;
	bool m_ignoreMissingTextures;
};