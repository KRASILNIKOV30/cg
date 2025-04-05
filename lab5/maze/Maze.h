#pragma once
#include "MazeScheme.h"
#include "../lib/Texture.h"
#include <memory>
#include <string>
#include <vector>
#include <glm/vec3.hpp>

class Maze
{
public:
	void Draw() const;
	[[nodiscard]] bool CanMoveTo(float x, float z) const;

private:
	void DrawFloor() const;
	void DrawWalls() const;
	void DrawCeiling() const;
	static void DrawWall(float x1, float z1, float x2, float z2);
	void LoadTextures() const;

	std::vector<std::vector<int>> m_maze = mazeScheme;
	glm::vec3 m_floorColor = { 0.3f, 0.3f, 0.3f };
	glm::vec3 m_ceilingColor = { 0.2f, 0.2f, 0.4f };
	float m_width = static_cast<float>(m_maze[0].size());
	float m_height = static_cast<float>(m_maze.size());

	std::vector<std::string> m_textureNames{
		"assets/chest.png",
		"assets/diamond.png",
		"assets/iron.png",
		"assets/iron.png",
		"assets/leaf.png",
		"assets/tnt.png",
	};
	mutable std::vector<std::unique_ptr<Texture2D>> m_textures;
	mutable bool m_texturesLoaded{ false };
};
