#include "Material.h"

Material::Material(void)
{
	SetDiffuse(1, 1, 1);
	SetAmbient(0.2f, 0.2f, 0.2f);
	SetSpecular(1, 1, 1);
	SetShininess(0);
}

void Material::SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_diffuse[0] = r;
	m_diffuse[1] = g;
	m_diffuse[2] = b;
	m_diffuse[3] = a;
}

void Material::SetAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_ambient[0] = r;
	m_ambient[1] = g;
	m_ambient[2] = b;
	m_diffuse[3] = a;
}

void Material::SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_specular[0] = r;
	m_specular[1] = g;
	m_specular[2] = b;
	m_specular[3] = a;
}

void Material::SetShininess(GLfloat shininess)
{
	m_shininess = shininess;
}

void Material::Activate(GLenum side) const
{
	glMaterialfv(side, GL_DIFFUSE, m_diffuse);
	glMaterialfv(side, GL_AMBIENT, m_ambient);
	glMaterialfv(side, GL_SPECULAR, m_specular);
	glMaterialf(side, GL_SHININESS, m_shininess);
}