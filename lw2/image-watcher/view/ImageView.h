#pragma once

#include "../model/ImageModel.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

class ImageView
{
public:
	void SetTransform(const sf::Vector2f& position, float scale);
	void UpdateTexture(const sf::Window& window, const ImageData& data);
	void Draw(sf::RenderTarget& target);

private:
	std::unique_ptr<sf::Texture> m_texture;
	std::optional<sf::Sprite> m_sprite;
};