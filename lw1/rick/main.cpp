#include "controller/RickController.h"
#include "model/RickModel.h"
#include "view/RickView.h"

#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Rick Sanchez");

	RickModel model;
	RickView view;
	RickController controller(model);

	while (window.isOpen())
	{
		while (auto eventOpt = window.pollEvent())
		{
			const sf::Event& event = *eventOpt;

			if (event.getIf<sf::Event::Closed>())
			{
				window.close();
			}

			controller.HandleEvent(event);
		}

		window.clear(sf::Color(50, 50, 80));
		RickView::Draw(model, window);
		window.display();
	}
	return 0;
}