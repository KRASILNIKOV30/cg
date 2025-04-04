#include "Surface.h"

Surface::Surface(int columns, int rows, float xMin, float xMax, float yMin, float yMax)
	: m_displayList(0)
	  , m_columns(columns)
	  , m_rows(rows)
	  , m_xMin(xMin)
	  , m_xMax(xMax)
	  , m_yMin(yMin)
	  , m_yMax(yMax)
{
}

void Surface::Draw() const
{
	glPolygonMode(GL_LINE_SMOOTH, GL_LINE);
	// При первом обращении к данной функции запишем команды
	// рисования поверхности в дисплейный список
	if (m_displayList == 0)
	{
		m_displayList = glGenLists(1);
		glNewList(m_displayList, GL_COMPILE);

		// вычисляем шаг узлов сетки
		const float dy = (m_yMax - m_yMin) / (m_rows - 1);
		const float dx = (m_xMax - m_xMin) / (m_columns - 1);

		float y = m_yMin;
		// пробегаем по строкам сетки
		for (int row = 0; row < m_rows - 1; ++row, y += dy)
		{
			// каждой строке будет соответствовать своя лента из треугольников
			glBegin(GL_TRIANGLE_STRIP);
			float x = m_xMin;

			// пробегаем по столбцам текущей строки
			for (int column = 0; column <= m_columns; ++column, x += dx)
			{
				// вычисляем параметры вершины в узлах пары соседних вершин
				// ленты из треугольников
				Vertex v0 = CalculateVertex(x, y + dy);
				Vertex v1 = CalculateVertex(x, y);

				// задаем нормаль и координаты вершины на четной позиции
				glNormal3fv(glm::value_ptr(v0.normal));
				glVertex3fv(value_ptr(v0.position));

				// задаем нормаль и координаты вершины на нечетной позиции
				glNormal3fv(value_ptr(v1.normal));
				glVertex3fv(value_ptr(v1.position));
			}
			glEnd();
		}

		glEndList();
	}

	// Вызовем ранее записанный дисплейный список команд рисования сетки
	glCallList(m_displayList);
}

Surface::~Surface()
{
	if (m_displayList != 0)
	{
		glDeleteLists(m_displayList, 1);
	}
}

Vertex Surface::CalculateVertex(double x, double y) const
{
	return {
		{ x, y, 0 },
		{ 0, 0, 0 },
	};
}