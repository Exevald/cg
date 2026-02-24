#include "ImageView.h"

void ImageView::SetTransform(const sf::Vector2f& position, float scale)
{
	if (m_sprite.has_value())
	{
		m_sprite->setPosition(position);
		m_sprite->setScale({ scale, scale });
	}
}

void ImageView::UpdateTexture(const sf::Window& window, const ImageData& data)
{
	if (!data.isValid || data.width == 0 || data.height == 0)
	{
		m_sprite.reset();
		m_texture.reset();
		return;
	}

	sf::Image img;
	img.resize({ window.getSize().x, window.getSize().y }, sf::Color(0, 0, 0, 0));

	for (unsigned int y = 0; y < data.height; ++y)
	{
		for (unsigned int x = 0; x < data.width; ++x)
		{
			unsigned int idx = (y * data.width + x) * 4;
			sf::Color color(
				data.pixels[idx],
				data.pixels[idx + 1],
				data.pixels[idx + 2],
				data.pixels[idx + 3]);
			img.setPixel({ x, y }, color);
		}
	}

	m_texture = std::make_unique<sf::Texture>();
	if (m_texture->loadFromImage(img))
	{
		m_sprite.emplace(*m_texture);
	}
}

void ImageView::Draw(sf::RenderTarget& target)
{
	if (m_sprite.has_value())
	{
		target.draw(*m_sprite);
	}
}
