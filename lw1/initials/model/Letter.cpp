#include "Letter.h"

#include <cmath>
#include <random>

namespace
{
std::mt19937& GetRandomGenerator()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return gen;
}
} // namespace

Letter::Letter(float x, float y, sf::Color color)
	: m_baseCoords({ x, y })
	, m_currentY(y)
	, m_color(color)
{

	std::uniform_real_distribution<float> dist(-700.0f, -400.0f);
	m_initialImpulse = dist(GetRandomGenerator());
}

void Letter::Update(float deltaTime)
{
	if (m_hasStopped)
	{
		return;
	}

	m_timer += deltaTime;
	if (!m_isActive)
	{
		m_velocity = m_initialImpulse;
		m_isActive = true;
	}

	m_velocity += m_gravity * deltaTime;
	m_currentY += m_velocity * deltaTime;

	if (m_currentY >= m_baseCoords.y)
	{
		m_currentY = m_baseCoords.y;
		// в чём измеряется m_dampingSpeed?
		m_velocity = -m_velocity * m_dampingSpeed;

		if (std::abs(m_velocity) < m_minVelocity)
		{
			m_velocity = 0.0f;
			m_currentY = m_baseCoords.y;
			m_hasStopped = true;
		}
	}
}