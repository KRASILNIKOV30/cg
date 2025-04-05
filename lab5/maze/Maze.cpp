#include "Maze.h"
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>

void Maze::Draw() const
{
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

	return m_maze[x][z] == 0;
}

void Maze::DrawFloor() const
{
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
	for (int x = 0; x < static_cast<int>(m_width); x++)
	{
		for (int z = 0; z < static_cast<int>(m_height); z++)
		{
			if (m_maze[x][z] == 1)
			{
				const auto xf = static_cast<float>(x);
				const auto zf = static_cast<float>(z);

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
	glBegin(GL_QUADS);
	glVertex3f(x1, 0, z1);
	glVertex3f(x1, 1, z1);
	glVertex3f(x2, 1, z2);
	glVertex3f(x2, 0, z2);
	glEnd();
}