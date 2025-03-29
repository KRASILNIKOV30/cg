#include "GreatDodecahedron.h"
#include <cmath>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>

GreatDodecahedron::GreatDodecahedron(float size)
	: m_size(size)
{
	constexpr glm::vec4 defaultColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	for (auto& color : m_faceColors)
	{
		color = defaultColor;
	}
	InitializeGeometry();
}

void GreatDodecahedron::InitializeGeometry()
{
	const float phi = (1.0f + std::sqrt(5.0f)) / 2.0f;
	m_vertices = {
		{ -1, phi, 0 }, { 1, phi, 0 }, { -1, -phi, 0 }, { 1, -phi, 0 },
		{ 0, -1, phi }, { 0, 1, phi }, { 0, -1, -phi }, { 0, 1, -phi },
		{ phi, 0, -1 }, { phi, 0, 1 }, { -phi, 0, -1 }, { -phi, 0, 1 }
	};

	for (auto& vertex : m_vertices)
	{
		vertex = glm::normalize(vertex);
	}

	m_faces = {
		{ 4, 9, 1, 0, 11 }, { 2, 10, 7, 8, 3 },
		{ 2, 4, 5, 0, 10 }, { 3, 6, 7, 1, 9 },
		{ 5, 9, 8, 7, 0 }, { 4, 11, 10, 6, 3 },
		{ 9, 4, 2, 6, 8 }, { 11, 5, 1, 7, 10 },
		{ 2, 3, 9, 5, 11 }, { 0, 1, 8, 6, 10 },
		{ 11, 0, 7, 6, 2 }, { 4, 3, 8, 1, 5 },
	};
}

void GreatDodecahedron::Draw() const
{
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(m_specularColor));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (const auto& face : m_faces)
	{
		glBegin(GL_LINE_LOOP);
		for (unsigned vi : face)
		{
			glVertex3fv(glm::value_ptr(m_vertices[vi] * m_size));
		}
		glEnd();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (size_t faceIdx = 0; faceIdx < m_faces.size(); ++faceIdx)
	{
		const auto& face = m_faces[faceIdx];
		glm::vec3 normal(0.0f);
		for (size_t i = 0; i < face.size(); ++i)
		{
			const auto& v0 = m_vertices[face[i]];
			const auto& v1 = m_vertices[face[(i + 1) % face.size()]];
			const auto& v2 = m_vertices[face[(i + 2) % face.size()]];
			normal += glm::cross(v1 - v0, v2 - v0);
		}
		normal = glm::normalize(normal);
		glBegin(GL_TRIANGLE_FAN);
		glNormal3fv(glm::value_ptr(normal));
		glColor4fv(glm::value_ptr(m_faceColors[faceIdx]));
		glm::vec3 center(0.0f);
		for (const unsigned vi : face)
		{
			center += m_vertices[vi];
		}
		center /= face.size();
		glVertex3fv(glm::value_ptr(center * m_size));

		for (const unsigned vi : face)
		{
			glVertex3fv(glm::value_ptr(m_vertices[vi] * m_size));
		}
		glVertex3fv(glm::value_ptr(m_vertices[face[0]] * m_size));
		glEnd();
	}
}

void GreatDodecahedron::SetFaceColor(DodecahedronFace face, const glm::vec4& color)
{
	m_faceColors[static_cast<size_t>(face)] = color;
}

void GreatDodecahedron::SetSpecularColor(const glm::vec4& color)
{
	m_specularColor = color;
}

void GreatDodecahedron::SetShininess(float shininess)
{
	m_shininess = shininess;
}