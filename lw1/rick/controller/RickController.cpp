#include "RickController.h"

RickController::RickController(RickModel& model)
	: m_model(model)
{
}

void RickController::HandleEvent(const sf::Event& event)
{
	if (const auto* buttonPressedEvent = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (buttonPressedEvent->button == sf::Mouse::Button::Left)
		{
			m_isDragging = true;
			m_lastMousePos = sf::Vector2f(
				static_cast<float>(buttonPressedEvent->position.x),
				static_cast<float>(buttonPressedEvent->position.y));
		}
	}
	else if (event.getIf<sf::Event::MouseButtonReleased>())
	{
		m_isDragging = false;
	}
	else if (const auto* mouseMovedEvent = event.getIf<sf::Event::MouseMoved>())
	{
		if (m_isDragging)
		{
			sf::Vector2f currentPos(
				static_cast<float>(mouseMovedEvent->position.x),
				static_cast<float>(mouseMovedEvent->position.y));
			sf::Vector2f delta = currentPos - m_lastMousePos;
			m_model.Move(delta);
			m_lastMousePos = currentPos;
		}
	}
}