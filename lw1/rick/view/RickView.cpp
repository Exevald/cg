#include "RickView.h"

void RickView::Draw(const RickModel& model, sf::RenderWindow& window)
{
	for (const auto& feature : model.GetFeatures())
	{
		feature.shapes->setFillColor(feature.color);
		window.draw(*feature.shapes);
	}
}