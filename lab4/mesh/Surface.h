#pragma once
#include <GL/gl.h>
#include <glm/ext.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::u8vec4 color;
};

class Surface
{
public:
	Surface(int columns, int rows, float xMin, float xMax, float yMin, float yMax);
	void Draw() const;
	virtual ~Surface();

protected:
	virtual Vertex CalculateVertex(double x, double y) const;

private:
	// Дисплейный список для хранения команд рисования поверхности
	mutable GLuint m_displayList;

	// количество строк и столбцов в сетке
	int m_columns;
	int m_rows;

	// область определения функции
	float m_xMin;
	float m_xMax;
	float m_yMin;
	float m_yMax;
};