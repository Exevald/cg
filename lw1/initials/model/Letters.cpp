#include "Letters.h"

LetterP::LetterP(float x, float y, sf::Color color)
	: Letter(x, y, color)
{

	auto left = std::make_unique<sf::RectangleShape>(sf::Vector2f(15, 100));
	auto right = std::make_unique<sf::RectangleShape>(sf::Vector2f(15, 100));
	auto top = std::make_unique<sf::RectangleShape>(sf::Vector2f(50, 15));

	left->setFillColor(color);
	right->setFillColor(color);
	top->setFillColor(color);

	m_parts.emplace_back(std::move(left), sf::Vector2f(0, 0));
	m_parts.emplace_back(std::move(right), sf::Vector2f(50, 0));
	m_parts.emplace_back(std::move(top), sf::Vector2f(0, 0));
}

LetterK::LetterK(float x, float y, sf::Color color)
	: Letter(x, y, color)
{
	auto stem = std::make_unique<sf::RectangleShape>(sf::Vector2f(20, 100));
	stem->setFillColor(color);
	m_parts.emplace_back(std::move(stem), sf::Vector2f(0, 0));

	auto topLeg = std::make_unique<sf::ConvexShape>();
	topLeg->setPointCount(4);
	topLeg->setPoint(0, sf::Vector2f(0, 0));
	topLeg->setPoint(1, sf::Vector2f(0, 20));
	topLeg->setPoint(2, sf::Vector2f(60, -10));
	topLeg->setPoint(3, sf::Vector2f(60, -30));
	topLeg->setFillColor(color);
	m_parts.emplace_back(std::move(topLeg), sf::Vector2f(20, 30));

	auto botLeg = std::make_unique<sf::ConvexShape>();
	botLeg->setPointCount(4);
	botLeg->setPoint(0, sf::Vector2f(0, 0));
	botLeg->setPoint(1, sf::Vector2f(0, 20));
	botLeg->setPoint(2, sf::Vector2f(60, 40));
	botLeg->setPoint(3, sf::Vector2f(60, 20));
	botLeg->setFillColor(color);
	m_parts.emplace_back(std::move(botLeg), sf::Vector2f(20, 50));
}

LetterO::LetterO(float x, float y, sf::Color color)
	: Letter(x, y, color)
{

	const float thickness = 20.f;
	const float width = 70.f;
	const float height = 100.f;

	auto left = std::make_unique<sf::RectangleShape>(sf::Vector2f(thickness, height));
	left->setFillColor(color);
	m_parts.emplace_back(std::move(left), sf::Vector2f(0, 0));

	auto right = std::make_unique<sf::RectangleShape>(sf::Vector2f(thickness, height));
	right->setFillColor(color);
	m_parts.emplace_back(std::move(right), sf::Vector2f(width - thickness, 0));

	auto top = std::make_unique<sf::RectangleShape>(sf::Vector2f(width - 2 * thickness, thickness));
	top->setFillColor(color);
	m_parts.emplace_back(std::move(top), sf::Vector2f(thickness, 0));

	auto bottom = std::make_unique<sf::RectangleShape>(sf::Vector2f(width - 2 * thickness, thickness));
	bottom->setFillColor(color);
	m_parts.emplace_back(std::move(bottom), sf::Vector2f(thickness, height - thickness));
}