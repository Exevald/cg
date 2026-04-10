#pragma once

#include <string>

struct Vec2
{
	float x, y;
};

struct RgbColor
{
	unsigned char r, g, b;
};

struct Element
{
	int id;
	std::string name;
	RgbColor color;
	bool isDiscovered;
};

struct ActiveItem
{
	int elementId;
	Vec2 position;
};