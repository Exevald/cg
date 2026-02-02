#pragma once

#include "../model/Letter.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class LetterView
{
public:
	static void Render(sf::RenderWindow& window, const std::vector<std::unique_ptr<Letter>>& letters);
};