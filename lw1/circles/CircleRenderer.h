#pragma once

#include <SFML/Graphics.hpp>

class CircleRenderer
{
public:
	CircleRenderer(unsigned int width, unsigned int height);

	void Clear(const sf::Color& color = sf::Color::Black);
	void SetPixel(int x, int y, const sf::Color& color);
	void DrawCircleOutline(int centerX, int centerY, int radius, const sf::Color& color);
	void FillCircle(int centerX, int centerY, int radius, const sf::Color& color);
	void DrawThickCircle(int centerX, int centerY, float radius, float thickness, const sf::Color& color);
	void DrawCircle(int centerX, int centerY, float radius, float thickness,
		const sf::Color& borderColor, const sf::Color& fillColor, bool filled);
	void Display(sf::RenderWindow& window);

private:
	sf::Image m_canvas;
};