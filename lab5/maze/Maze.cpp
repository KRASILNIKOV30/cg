#include "Maze.h"

#include "../lib/TextureLoader.h"

#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>

void Maze::Draw() const
{
	if (!m_texturesLoaded)
	{
		LoadTextures();
	}
	DrawFloor();
	DrawCeiling();
	DrawWalls();
}

bool Maze::CanMoveTo(float x, float z) const
{
	if (x < 0 || x >= m_width || z < 0 || z >= m_height)
	{
		return false;
	}

	return m_maze[static_cast<int>(x)][static_cast<int>(z)] == 0;
}

void Maze::DrawFloor() const
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(m_floorColor.r, m_floorColor.g, m_floorColor.b);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(m_width, 0, 0);
	glVertex3f(m_width, 0, m_height);
	glVertex3f(0, 0, m_height);
	glEnd();
}

void Maze::DrawCeiling() const
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(m_ceilingColor.r, m_ceilingColor.g, m_ceilingColor.b);
	glBegin(GL_QUADS);
	glVertex3f(0, 1, 0);
	glVertex3f(m_width, 1, 0);
	glVertex3f(m_width, 1, m_height);
	glVertex3f(0, 1, m_height);
	glEnd();
}

void Maze::DrawWalls() const
{
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	for (int x = 0; x < static_cast<int>(m_width); x++)
	{
		for (int z = 0; z < static_cast<int>(m_height); z++)
		{
			const auto value = m_maze[x][z];
			if (value)
			{
				const auto xf = static_cast<float>(x);
				const auto zf = static_cast<float>(z);

				m_textures[value - 1]->Bind();

				DrawWall(xf, zf, xf, zf + 1);
				DrawWall(xf + 1, zf, xf + 1, zf + 1);
				DrawWall(xf, zf, xf + 1, zf);
				DrawWall(xf, zf + 1, xf + 1, zf + 1);
			}
		}
	}
}

void Maze::DrawWall(float x1, float z1, float x2, float z2)
{
	// Меньше bind (исправлено)
	glBegin(GL_TRIANGLE_STRIP);

	glTexCoord2f(0, 1);
	glVertex3f(x1, 0, z1);

	glTexCoord2f(0, 0);
	glVertex3f(x1, 1, z1);

	glTexCoord2f(1, 1);
	glVertex3f(x2, 0, z2);

	glTexCoord2f(1, 0);
	glVertex3f(x2, 1, z2);

	glEnd();
}

void Maze::LoadTextures() const
{
	m_textures.reserve(m_textureNames.size());
	TextureLoader loader;
	for (const auto& name : m_textureNames)
	{
		m_textures.emplace_back(std::make_unique<Texture2D>());
		m_textures.back()->Attach(loader.LoadTexture2D(name));
	}
	m_texturesLoaded = true;
}