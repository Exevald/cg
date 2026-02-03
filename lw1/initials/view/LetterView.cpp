#include "LetterView.h"

void LetterView::Render(sf::RenderWindow& window, const std::vector<std::unique_ptr<Letter>>& letters)
{
	for (const auto& letter : letters)
	{
		float x = letter->GetX();
		float y = letter->GetY();
		sf::Color color = letter->GetColor();

		for (const auto& part : letter->GetParts())
		{
			part.shape->setPosition(sf::Vector2f(
				x + part.offset.x,
				y + part.offset.y));
			part.shape->setFillColor(color);
			window.draw(*part.shape);
		}
	}
}
