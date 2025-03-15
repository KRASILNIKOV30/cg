#pragma once
#include "../model/Model.h"
#include <GL/glut.h>

struct Color
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
};

constexpr int BLOCK_SIZE = 30;
constexpr int BLOCKS_IN_ROW = FIELD_WIDTH + 2;
constexpr int BLOCKS_IN_COLUMN = FIELD_HEIGHT + 1;
constexpr int BLOCKS_NUMBER = BLOCKS_IN_ROW * BLOCKS_IN_COLUMN;
constexpr Color BORDER_COLOR = {
	.r = 0.5f,
	.g = 0.5f,
	.b = 0.55f,
};

class View
{
public:
	explicit View(const Model& m)
		: m_model(m)
	{
	}

	void Render(int width, int height)
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glViewport(100, 100, BLOCK_SIZE * BLOCKS_IN_ROW, BLOCK_SIZE * BLOCKS_IN_COLUMN);

		DrawBorder();
		for (int y = 0; y < FIELD_HEIGHT; ++y)
		{
			for (int x = 0; x < FIELD_WIDTH; ++x)
			{
				const auto block = m_model.GetBlock(x, y);
				if (block)
				{
					const auto color = GetColor(block);
					DrawBlock(x + 1, y, color);
				}
			}
		}

		DrawStats();
	}

	void DrawBorder()
	{
		for (int i = 0; i < BLOCKS_NUMBER; i++)
		{
			const int x = i % BLOCKS_IN_ROW;
			const int y = i / BLOCKS_IN_ROW;
			if (x == 0 || x == BLOCKS_IN_ROW - 1 || y == BLOCKS_IN_COLUMN - 1)
			{
				DrawBlock(x, y, BORDER_COLOR);
			}
		}
	}

private:
	static void DrawBlock(int x, int y, Color color)
	{
		constexpr auto width = 2.0f / BLOCKS_IN_ROW;
		constexpr auto height = 2.0f / BLOCKS_IN_COLUMN;
		const auto posX = -1.0f + x * width;
		const auto posY = 1.0f - y * height;

		DrawRectangle(posX, posY, width, height, color);
	}

	static void DrawRectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height, Color color)
	{
		const float horBorder = width / 20;
		const float verBorder = height / 20;
		const float w = width - 2 * horBorder;
		const float h = height - 2 * verBorder;
		const auto posX = x + horBorder;
		const auto posY = y - verBorder;
		const auto [r, g, b] = color;

		glBegin(GL_TRIANGLE_STRIP);

		glColor3f(r, g, b);

		glVertex2f(posX, posY);
		glVertex2f(posX + w, posY);
		glVertex2f(posX, posY - h);
		glVertex2f(posX + w, posY - h);

		glEnd();
	}

	void DrawStats()
	{
		glViewport(250, 200, 500, 350);
		glColor3f(1.0f, 1.0f, 1.0f);

		const auto score = "Score: " + std::to_string(m_model.GetScore());
		const auto level = "Level: " + std::to_string(m_model.GetLevel());
		const auto linesLeft = "Lines left: " + std::to_string(m_model.GetLinesLeft());
		const auto next = "Next: ";

		const auto isGameOver = m_model.IsGameOver();
		const auto gameOver = isGameOver ? "Game over" : "";

		DrawText(score, 0.0f, 1.0f);
		DrawText(level, 0.0f, 0.75f);
		DrawText(linesLeft, 0.0f, 0.5f);
		DrawText(next, 0.0f, 0.25f);
		DrawText(gameOver, 0.0f, 0.0f);

		if (!isGameOver)
		{
			DrawNext();
		}
	}

	void DrawNext() const
	{
		const auto next = m_model.GetNextTetromino();
		const auto size = next.size();
		const auto viewportSize = static_cast<int>(BLOCK_SIZE * size);
		glViewport(570, 350, viewportSize, viewportSize);

		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				const auto block = next[y][x];
				if (block)
				{
					const auto blockSide = 2.0f / static_cast<float>(size);
					const auto posX = -1.0f + static_cast<float>(x) * blockSide;
					const auto posY = 1.0f - static_cast<float>(y) * blockSide;
					const auto color = GetColor(block);
					DrawRectangle(posX, posY, blockSide, blockSide, color);
				}
			}
		}
	}

	static void DrawText(std::string const& text, GLfloat x, GLfloat y)
	{
		glRasterPos2f(x, y);
		for (char c : text)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
		}
	}

	static Color GetColor(int tetrominoType)
	{
		if (tetrominoType == 1)
			return { 1.0f, 0.0f, 0.0f };
		if (tetrominoType == 2)
			return { 0.0f, 1.0f, 0.0f };
		if (tetrominoType == 3)
			return { 0.0f, 0.0f, 1.0f };
		if (tetrominoType == 4)
			return { 1.0f, 1.0f, 0.0f };
		if (tetrominoType == 5)
			return { 0.0f, 1.0f, 1.0f };
		if (tetrominoType == 6)
			return { 1.0f, 0.0f, 1.0f };
		if (tetrominoType == 7)
			return { 1.0f, 1.0f, 1.0f };

		return { 0.0f, 0.0f, 0.0f };
	}

private:
	const Model& m_model;
};