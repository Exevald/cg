#pragma once

#include "../model/AlchemyModel.h"
#include "../view/SFMLView.h"

class AlchemyController
{
public:
	AlchemyController(AlchemyModel& m, SFMLView& v)
		: m_model(m)
		, m_view(v)
	{
	}
	void ProcessInput();

private:
	void HandleMouseButtonPressed(const sf::Event::MouseButtonPressed* mousePressed);
	void HandleMouseButtonReleased(const sf::Event::MouseButtonReleased* mouseReleased) const;
	void HandleMouseMoved(const sf::Event::MouseMoved* mouseMoved) const;

	AlchemyModel& m_model;
	SFMLView& m_view;
	Vec2 m_dragOffset{ 0, 0 };
};