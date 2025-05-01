#pragma once

namespace v
{
struct Vector3
{
	float x, y, z;
};

struct Vector2
{
	float x, y;
};
}

struct Vertex
{
	v::Vector3 position;
	v::Vector3 normal;
};

struct TexturedVertex : Vertex
{
	v::Vector2 texCoord;
};