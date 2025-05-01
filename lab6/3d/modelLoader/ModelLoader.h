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

private:
	void ProcessMaterials(const tinyobj::attrib_t& attrib,
		const std::vector<tinyobj::material_t>& materials,
		Model& model) const;

	void ProcessMesh(const tinyobj::attrib_t& attrib,
		const std::vector<tinyobj::shape_t>& shapes,
		Model& model) const;

	GLenum m_vertexBufferUsage;
	GLenum m_indexBufferUsage;
	bool m_ignoreMissingTextures;
};