#include "CircleRenderer.h"

#include <algorithm>
#include <cmath>

CircleRenderer::CircleRenderer(unsigned int width, unsigned int height)
	: m_canvas({ width, height }, sf::Color::White)
{
}

void CircleRenderer::Clear(const sf::Color& color)
{
	auto size = m_canvas.getSize();
	for (unsigned int y = 0; y < size.y; ++y)
	{
		for (unsigned int x = 0; x < size.x; ++x)
		{
			m_canvas.setPixel({ x, y }, color);
		}
	}
}

void CircleRenderer::SetPixel(int x, int y, const sf::Color& color)
{
	auto size = m_canvas.getSize();
	if (x >= 0 && static_cast<unsigned int>(x) < size.x && y >= 0 && static_cast<unsigned int>(y) < size.y)
	{
		m_canvas.setPixel({ static_cast<unsigned int>(x), static_cast<unsigned int>(y) }, color);
	}
}

void CircleRenderer::DrawCircleOutline(int centerX, int centerY, int radius, const sf::Color& color)
{
	if (radius <= 0)
	{
		return;
	}

	int x = radius;
	int y = 0;
	int err = 1 - x;

	while (x >= y)
	{
		SetPixel(centerX + x, centerY + y, color);
		SetPixel(centerX + y, centerY + x, color);
		SetPixel(centerX - y, centerY + x, color);
		SetPixel(centerX - x, centerY + y, color);
		SetPixel(centerX - x, centerY - y, color);
		SetPixel(centerX - y, centerY - x, color);
		SetPixel(centerX + y, centerY - x, color);
		SetPixel(centerX + x, centerY - y, color);

		y++;
		if (err < 0)
		{
			err += 2 * y + 1;
		}
		else
		{
			x--;
			err += 2 * (y - x + 1);
		}
	}
}

void CircleRenderer::FillCircle(int centerX, int centerY, int radius, const sf::Color& color)
{
	if (radius <= 0)
	{
		return;
	}

	int radiusSquared = radius * radius;
	auto size = m_canvas.getSize();
	int minY = std::max(0, centerY - radius);
	int maxY = std::min(static_cast<int>(size.y) - 1, centerY + radius);

	for (int y = minY; y <= maxY; ++y)
	{
		int dy = y - centerY;
		int dx = static_cast<int>(std::sqrt(radiusSquared - dy * dy));
		int minX = std::max(0, centerX - dx);
		int maxX = std::min(static_cast<int>(size.x) - 1, centerX + dx);
		for (int x = minX; x <= maxX; ++x)
		{
			SetPixel(x, y, color);
		}
	}
}

void CircleRenderer::DrawThickCircle(int centerX, int centerY, float radius, float thickness, const sf::Color& color)
{
	int outerRadius = static_cast<int>(std::ceil(radius + thickness / 2.0f));
	int innerRadius = static_cast<int>(std::floor(radius - thickness / 2.0f));
	if (innerRadius < 0)
	{
		innerRadius = 0;
	}

	for (int r = innerRadius; r <= outerRadius; ++r)
	{
		DrawCircleOutline(centerX, centerY, r, color);
	}
}

void CircleRenderer::DrawCircle(int centerX, int centerY, float radius, float thickness,
	const sf::Color& borderColor, const sf::Color& fillColor, bool filled)
{
	if (filled && radius > thickness)
	{
		int fillRadius = static_cast<int>(radius - thickness);
		if (fillRadius > 0)
		{
			FillCircle(centerX, centerY, fillRadius, fillColor);
		}
	}

	if (thickness < 1.0f)
	{
		DrawCircleOutline(centerX, centerY, static_cast<int>(radius), borderColor);
	}
	else
	{
		DrawThickCircle(centerX, centerY, radius, thickness, borderColor);
	}
}

void CircleRenderer::Display(sf::RenderWindow& window)
{
	sf::Texture texture;
	if (!texture.loadFromImage(m_canvas))
	{
		return;
	}

	sf::Sprite sprite(texture);
	window.draw(sprite);
}