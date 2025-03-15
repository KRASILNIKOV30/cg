#pragma once
#include <stdexcept>
#include <vector>
#include <random>

struct Point
{
	int x;
	int y;
};

using Tetromino = std::vector<std::vector<int>>;

inline Tetromino GetRandomTetromino()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	// вынести в константы
	std::uniform_int_distribution dis(1, 7);
	int type = dis(gen);
	if (type == 1) // L
	{
		return {
			std::vector{ 1, 0, 0 },
			std::vector{ 1, 0, 0 },
			std::vector{ 1, 1, 0 },
		};
	}
	if (type == 2) // J
	{
		return {
			std::vector{ 0, 0, 2 },
			std::vector{ 0, 0, 2 },
			std::vector{ 0, 2, 2 },
		};
	}
	if (type == 3) // O
	{
		return {
			std::vector{ 3, 3 },
			std::vector{ 3, 3 },
		};
	}
	if (type == 4) // I
	{
		return {
			std::vector{ 0, 4, 0, 0 },
			std::vector{ 0, 4, 0, 0 },
			std::vector{ 0, 4, 0, 0 },
			std::vector{ 0, 4, 0, 0 }
		};
	}
	if (type == 5) // 5
	{
		return {
			std::vector{ 0, 5, 5 },
			std::vector{ 5, 5, 0 },
			std::vector{ 0, 0, 0 },
		};
	}
	if (type == 6) // Z
	{
		return {
			std::vector{ 6, 6, 0 },
			std::vector{ 0, 6, 6 },
			std::vector{ 0, 0, 0 }
		};
	}
	if (type == 7) // T
	{
		return {
			std::vector{ 7, 7, 7 },
			std::vector{ 0, 7, 0 },
			std::vector{ 0, 0, 0 },
		};
	}
	throw std::runtime_error("Unknown tetromino type");
}
