#pragma once
#include "Tetromino.h"

#include <array>
#include <functional>
#include <valarray>
#include <bits/ranges_algo.h>

constexpr int FIELD_WIDTH = 10;
constexpr int FIELD_HEIGHT = 20;

class Model
{
public:
	using Field = std::array<std::array<int, FIELD_WIDTH>, FIELD_HEIGHT>;

	Model()
	{
		m_nextTetromino = GetRandomTetromino();
		SpawnPiece();
	}

	void Update()
	{
		if (!MoveDown())
		{
			LockPiece();
			ClearLines();
			SpawnPiece();
			if (IsCollision(m_tetromino, 0, 1))
			{
				isGameOver = true;
			}
		}
	}

	[[nodiscard]] bool IsGameOver() const
	{
		return isGameOver;
	}

	[[nodiscard]] int GetBlock(int x, int y) const
	{
		const auto tx = x - m_tetrominoPos.x;
		const auto ty = y - m_tetrominoPos.y;
		const auto tSize = m_tetromino.size();
		if (tx >= 0 && tx < tSize && ty >= 0 && ty < tSize && m_tetromino[ty][tx] != 0)
		{
			return m_tetromino[ty][tx];
		}
		return m_field[y][x];
	}

	[[nodiscard]] int GetScore() const
	{
		return m_score;
	}

	void MoveLeft()
	{
		Move(-1, 0);
	}

	void MoveRight()
	{
		Move(1, 0);
	}

	bool MoveDown()
	{
		return Move(0, 1);
	}

	void Rotate()
	{
		const auto size = m_tetromino.size();
		const auto n = size - 1;
		Tetromino rotated = m_tetromino;
		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				rotated[y][x] = m_tetromino[n - x][y];
			}
		}
		if (!IsCollision(rotated, 0, 0))
		{
			m_tetromino = rotated;
		}
	}

private:
	bool Move(int offsetX, int offsetY)
	{
		const auto collision = IsCollision(m_tetromino, offsetX, offsetY);
		if (!collision)
		{
			m_tetrominoPos.x += offsetX;
			m_tetrominoPos.y += offsetY;
		}
		return !collision;
	}

	void LockPiece()
	{
		const auto size = m_tetromino.size();
		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				const auto block = m_tetromino[y][x];
				if (block)
				{
					m_field[m_tetrominoPos.y + y][m_tetrominoPos.x + x] = block;
				}
			}
		}
	}

	void ClearLines()
	{
		int fullLines = 0;
		for (int i = FIELD_HEIGHT - 1; i >= 0; --i)
		{
			const auto isLineFull = std::ranges::all_of(m_field[i], [&](int b) { return b; });
			if (isLineFull)
			{
				for (int k = i; k > 0; --k)
				{
					m_field[k] = m_field[k - 1];
				}
				m_field[0] = std::array<int, FIELD_WIDTH>{};
				++fullLines;
			}
		}
		m_score += static_cast<int>(std::pow(2, fullLines)) - 10;
	}

	void SpawnPiece()
	{
		m_tetrominoPos = { 3, 0 };
		m_tetromino = m_nextTetromino;
		m_nextTetromino = GetRandomTetromino();
	}

	[[nodiscard]] bool IsCollision(const Tetromino& tetromino, int offsetX, int offsetY) const
	{
		bool result = false;
		const auto size = tetromino.size();
		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				const auto block = tetromino[y][x];
				if (!block)
				{
					continue;
				}
				const auto newX = x + m_tetrominoPos.x + offsetX;
				const auto newY = y + m_tetrominoPos.y + offsetY;
				result |= newX < 0 || newX >= FIELD_WIDTH || newY >= FIELD_HEIGHT || m_field[newY][newX];
			}
		}
		return result;
	}

private:
	Field m_field{};
	Tetromino m_tetromino;
	Tetromino m_nextTetromino;
	Point m_tetrominoPos{};
	int m_score = 0;
	bool isGameOver = false;
	bool isPause = false;
};
