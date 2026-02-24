#include "controller/LetterController.h"
#include "model/Letters.h"
#include "view/LetterView.h"

#include <memory>
#include <vector>

struct WindowSettings
{
	std::string windowTitle = "Initials";
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;
};

int main()
{
	WindowSettings settings;
	sf::RenderWindow window(
		sf::VideoMode(sf::Vector2u(settings.windowWidth, settings.windowHeight)),
		settings.windowTitle);
	window.setFramerateLimit(60);

	std::vector<std::unique_ptr<Letter>> letters;

	letters.push_back(std::make_unique<LetterK>(100, 300, sf::Color::White));
	letters.push_back(std::make_unique<LetterK>(200, 300, sf::Color::Yellow));
	letters.push_back(std::make_unique<LetterP>(300, 300, sf::Color::Red));
	letters.push_back(std::make_unique<LetterO>(400, 300, sf::Color::Blue));

	sf::Clock clock;
	while (window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();

		while (auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>()
				|| (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
			{
				window.close();
			}
		}

		LetterController::Update(letters, deltaTime);
		window.clear(sf::Color(30, 30, 30));
		LetterView::Render(window, letters);
		window.display();
	}

	return EXIT_SUCCESS;
}