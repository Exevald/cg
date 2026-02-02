#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

struct WindowSettings
{
	std::string windowTitle = "Initials";
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;
};

struct Coords
{
	float x, y;
};

struct LetterPart
{
	std::unique_ptr<sf::Shape> shape;
	sf::Vector2f offset;
};