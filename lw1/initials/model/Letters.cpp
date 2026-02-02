#include "Letters.h"

LetterP::LetterP(float x, float y, sf::Color color, float delay)
	: Letter(x, y, color, delay)
{
	auto left = std::make_unique<sf::RectangleShape>(sf::Vector2f(15, 100));
	auto right = std::make_unique<sf::RectangleShape>(sf::Vector2f(15, 100));
	auto top = std::make_unique<sf::RectangleShape>(sf::Vector2f(50, 15));

	m_parts.push_back({ std::move(left), { 0, 0 } });
	m_parts.push_back({ std::move(right), { 50, 0 } });
	m_parts.push_back({ std::move(top), { 0, 0 } });
}

LetterK::LetterK(float x, float y, sf::Color color, float delay)
	: Letter(x, y, color, delay)
{
	auto stem = std::make_unique<sf::RectangleShape>(sf::Vector2f(20, 100));
	m_parts.push_back({ std::move(stem), { 0, 0 } });

	auto topLeg = std::make_unique<sf::ConvexShape>(4);
	topLeg->setPoint(0, { 10, 50 });
	topLeg->setPoint(1, { 10, 30 });
	topLeg->setPoint(2, { 70, 0 });
	topLeg->setPoint(3, { 70, 20 });
	m_parts.push_back({ std::move(topLeg), { 0, 0 } });

	auto botLeg = std::make_unique<sf::ConvexShape>(4);
	botLeg->setPoint(0, { 20, 50 });
	botLeg->setPoint(1, { 20, 70 });
	botLeg->setPoint(2, { 70, 100 });
	botLeg->setPoint(3, { 70, 75 });
	m_parts.push_back({ std::move(botLeg), { 0, 0 } });
}

LetterO::LetterO(float x, float y, sf::Color color, float delay)
	: Letter(x, y, color, delay)
{
	float thickness = 20.f;
	float width = 70.f;
	float height = 100.f;

	auto left = std::make_unique<sf::RectangleShape>(sf::Vector2f(thickness, height));
	m_parts.push_back({ std::move(left), { 0, 0 } });

	auto right = std::make_unique<sf::RectangleShape>(sf::Vector2f(thickness, height));
	m_parts.push_back({ std::move(right), { width - thickness, 0 } });

	auto top = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, thickness));
	m_parts.push_back({ std::move(top), { 0, 0 } });

	auto bottom = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, thickness));
	m_parts.push_back({ std::move(bottom), { 0, height - thickness } });
}
