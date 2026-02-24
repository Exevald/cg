#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

struct LetterPart
{
	std::unique_ptr<sf::Shape> shape;
	sf::Vector2f offset;

	LetterPart(std::unique_ptr<sf::Shape> s, sf::Vector2f off)
		: shape(std::move(s))
		, offset(off)
	{
	}

	LetterPart(const LetterPart&) = delete;
	LetterPart& operator=(const LetterPart&) = delete;
	LetterPart(LetterPart&&) noexcept = default;
};

class Letter
{
public:
	explicit Letter(float x, float y, sf::Color color);

	void Update(float deltaTime);

	[[nodiscard]] sf::Vector2f GetPosition() const { return { m_baseCoords.x, m_currentY }; }
	[[nodiscard]] const std::vector<LetterPart>& GetParts() const { return m_parts; }
	[[nodiscard]] sf::Color GetColor() const { return m_color; }

protected:
	std::vector<LetterPart> m_parts;

private:
	sf::Vector2f m_baseCoords;
	float m_currentY;
	sf::Color m_color;

	float m_velocity = 0.0f;
	float m_gravity = 1200.0f;
	float m_dampingSpeed = 0.78f;
	float m_minVelocity = 30.0f;
	float m_initialImpulse = 0.0f;

	float m_timer = 0.0f;
	bool m_isActive = false;
	bool m_hasStopped = false;
};