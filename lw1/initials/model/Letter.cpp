#include "Letter.h"

Letter::Letter(float x, float y, sf::Color color, float delay)
	: m_baseCoords({ x, y })
	, m_currentY(y)
	, m_color(color)
	, m_phaseDelay(delay)
{
}

void Letter::Update(float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer < m_phaseDelay)
	{
		return;
	}

	constexpr float speedMultiplier = 60.0f;
	m_velocity += m_gravity * speedMultiplier * deltaTime;
	m_currentY += m_velocity * speedMultiplier * deltaTime;

	if (m_currentY >= m_baseCoords.y)
	{
		m_currentY = m_baseCoords.y;
		m_velocity = m_jumpStrength;
	}
}