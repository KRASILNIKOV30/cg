#pragma once
#include <glm/glm.hpp>
#include <vector>

enum class DodecahedronFace
{
	FACE_0, FACE_1, FACE_2, FACE_3, FACE_4, FACE_5,
	FACE_6, FACE_7, FACE_8, FACE_9, FACE_10, FACE_11
};

class GreatDodecahedron
{
public:
	explicit GreatDodecahedron(float size = 1.0f);
	void Draw() const;

	void SetFaceColor(DodecahedronFace face, const glm::vec4& color);
	void SetSpecularColor(const glm::vec4& color);
	void SetShininess(float shininess);

private:
	float m_size;
	glm::vec4 m_faceColors[12]{};
	glm::vec4 m_specularColor = { 0, 0, 0, 1 };
	float m_shininess = 1.0f;

	void InitializeGeometry();
	std::vector<glm::vec3> m_vertices;
	std::vector<std::vector<unsigned>> m_faces;
};