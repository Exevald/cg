#include "CircleRenderer.h"

#include <SFML/Graphics.hpp>

namespace
{

constexpr unsigned int WindowWidth = 1024;
constexpr unsigned int WindowHeight = 768;
constexpr float DefaultRadius = 150.0f;
constexpr float DefaultThickness = 3.0f;

void HandleEvents(sf::RenderWindow& window,
	bool& demoMode, bool& filled, float& centerX, float& centerY, float& radius, float& thickness)
{
	while (auto event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
			return;
		}

		if (event->is<sf::Event::KeyPressed>())
		{
			auto key = event->getIf<sf::Event::KeyPressed>();
			switch (key->code)
			{
			case sf::Keyboard::Key::Escape:
				window.close();
				break;
			case sf::Keyboard::Key::F:
				filled = !filled;
				break;
			case sf::Keyboard::Key::D:
				demoMode = !demoMode;
				break;
			case sf::Keyboard::Key::T:
				thickness = std::max(0.5f, thickness - 0.5f);
				break;
			case sf::Keyboard::Key::Y:
				thickness = std::min(50.0f, thickness + 0.5f);
				break;
			case sf::Keyboard::Key::R:
				centerX = WindowWidth / 2.0f;
				centerY = WindowHeight / 2.0f;
				radius = DefaultRadius;
				thickness = DefaultThickness;
				break;
			default:
				break;
			}
		}

		if (!demoMode)
		{
			if (event->is<sf::Event::MouseMoved>())
			{
				auto mouse = event->getIf<sf::Event::MouseMoved>();
				centerX = static_cast<float>(mouse->position.x);
				centerY = static_cast<float>(mouse->position.y);
			}
			else if (event->is<sf::Event::MouseWheelScrolled>())
			{
				auto wheel = event->getIf<sf::Event::MouseWheelScrolled>();
				radius = std::max(5.0f, radius + wheel->delta * 10.0f);
			}
		}
	}
}

void RenderDemoMode(CircleRenderer& renderer)
{
	renderer.DrawCircle(
		WindowWidth / 2,
		WindowHeight / 2,
		200,
		5,
		sf::Color::Cyan,
		sf::Color::Blue,
		true);
	renderer.DrawCircle(
		200,
		200,
		80,
		2,
		sf::Color::Yellow,
		sf::Color::Red,
		true);
	// todo: переделать
	renderer.DrawCircle(
		800,
		600,
		55,
		45,
		sf::Color::Green,
		sf::Color::Magenta,
		false);
}

void RenderSingleCircle(CircleRenderer& renderer, float centerX, float centerY,
	float radius, float thickness,
	const sf::Color& borderColor,
	const sf::Color& fillColor, bool filled)
{
	renderer.DrawCircle(
		static_cast<int>(centerX),
		static_cast<int>(centerY),
		radius,
		thickness,
		borderColor,
		fillColor,
		filled);
}

} // namespace

int main()
{
	sf::VideoMode mode({ WindowWidth, WindowHeight });
	sf::RenderWindow window(mode, "Circles", sf::Style::Default);
	window.setFramerateLimit(60);

	CircleRenderer renderer(WindowWidth, WindowHeight);

	float centerX = WindowWidth / 2.0f;
	float centerY = WindowHeight / 2.0f;
	float radius = DefaultRadius;
	float thickness = DefaultThickness;
	bool filled = true;
	bool demoMode = true;

	sf::Color borderColor(255, 100, 100);
	sf::Color fillColor(100, 100, 255);

	while (window.isOpen())
	{
		HandleEvents(window, demoMode, filled, centerX, centerY, radius, thickness);

		renderer.Clear(sf::Color(30, 30, 40));

		if (demoMode)
		{
			RenderDemoMode(renderer);
		}
		else
		{
			RenderSingleCircle(renderer, centerX, centerY, radius, thickness, borderColor, fillColor, filled);
		}

		renderer.Display(window);
		window.display();
	}

	return 0;
}