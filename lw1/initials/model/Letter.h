#pragma once

#include "Common.h"
#include <vector>

class Letter
{
public:
	Letter(float x, float y, sf::Color color, float delay);
	virtual ~Letter() = default;

	void Update(float deltaTime);

	[[nodiscard]] const std::vector<LetterPart>& GetParts() const { return m_parts; }
	[[nodiscard]] float GetX() const { return m_baseCoords.x; }
	[[nodiscard]] float GetY() const { return m_currentY; }
	[[nodiscard]] sf::Color GetColor() const { return m_color; }

protected:
	std::vector<LetterPart> m_parts;
	Coords m_baseCoords;
	float m_currentY;
	sf::Color m_color;

	float m_velocity = 0;
	float m_gravity = 0.5f;
	float m_jumpStrength = -12.0f;
	float m_phaseDelay;
	float m_timer = 0;
};