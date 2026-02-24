#pragma once

#include "../model/RickModel.h"
#include <SFML/Graphics.hpp>

class RickController {
public:
	explicit RickController(RickModel& model);
	void HandleEvent(const sf::Event& event);

private:
	RickModel& m_model;
	bool m_isDragging = false;
	sf::Vector2f m_lastMousePos;
};