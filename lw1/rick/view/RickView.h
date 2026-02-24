#pragma once

#include "../model/RickModel.h"

#include <SFML/Graphics.hpp>

class RickView
{
public:
	static void Draw(const RickModel& model, sf::RenderWindow& window);
};