#include "controller/LetterController.h"
#include "model/Common.h"
#include "model/Letters.h"
#include "view/LetterView.h"

#include <memory>
#include <vector>

int main()
{
	WindowSettings settings;
	sf::RenderWindow window(
		sf::VideoMode(
			settings.windowWidth,
			settings.windowHeight),
		settings.windowTitle);
	window.setFramerateLimit(60);

	std::vector<std::unique_ptr<Letter>> letters;

	letters.push_back(std::make_unique<LetterK>(100, 300, sf::Color::White, 0.0f));
	letters.push_back(std::make_unique<LetterK>(200, 300, sf::Color::Yellow, 0.1f));
	letters.push_back(std::make_unique<LetterP>(300, 300, sf::Color::Red, 0.2f));
	letters.push_back(std::make_unique<LetterO>(400, 300, sf::Color::Blue, 0.3f));

	sf::Clock clock;
	while (window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();

		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
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