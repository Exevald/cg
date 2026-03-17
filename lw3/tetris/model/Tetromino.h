#pragma once

#include <random>
#include <vector>

struct Point
{
	int x, y;
};
using Tetromino = std::vector<std::vector<int>>;

struct Color
{
	float r, g, b;
};

inline Color GetColor(const int type)
{
	switch (type)
	{
	case 1:
		return { 1.0f, 0.0f, 0.0f };
	case 2:
		return { 0.0f, 0.0f, 1.0f };
	case 3:
		return { 1.0f, 1.0f, 0.0f };
	case 4:
		return { 0.0f, 1.0f, 1.0f };
	case 5:
		return { 0.0f, 1.0f, 0.0f };
	case 6:
		return { 1.0f, 0.0f, 1.0f };
	case 7:
		return { 1.0f, 0.5f, 0.0f };
	default:
		return { 0.5f, 0.5f, 0.5f };
	}
}

inline Tetromino GetRandomTetromino()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 7);
	const int type = dis(gen);

	if (type == 1)
	{
		return {
			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 1, 1, 0 }
		};
	}
	if (type == 2)
	{
		return {
			{ 0, 0, 2 },
			{ 0, 0, 2 },
			{ 0, 2, 2 }
		};
	}
	if (type == 3)
	{
		return {
			{ 3, 3 },
			{ 3, 3 }
		};
	}
	if (type == 4)
	{
		return {
			{ 0, 4, 0, 0 },
			{ 0, 4, 0, 0 },
			{ 0, 4, 0, 0 },
			{ 0, 4, 0, 0 }
		};
	}
	if (type == 5)
	{
		return {
			{ 0, 5, 5 },
			{ 5, 5, 0 },
			{ 0, 0, 0 }
		};
	}
	if (type == 6)
	{
		return {
			{ 6, 6, 0 },
			{ 0, 6, 6 },
			{ 0, 0, 0 }
		};
	}
	return {
		{ 7, 7, 7 },
		{ 0, 7, 0 },
		{ 0, 0, 0 }
	};
}