#include "SFMLView.h"

#include <iostream>

SFMLView::SFMLView(const AlchemyModel& model)
	: m_window(sf::VideoMode({ 1000, 700 }), "WoW Alchemy")
	, m_recycleBinText(m_font, "")
{
	m_window.setFramerateLimit(60);
	if (!m_font.openFromFile("arial.ttf"))
	{
		throw std::runtime_error("Failed to load font");
	}

	LoadTextures(model);

	m_sidebar.setSize({ 300.f, 700.f });
	m_sidebar.setFillColor(sf::Color(30, 30, 30));

	m_recycleBin.setSize({ 80.f, 80.f });
	m_recycleBin.setPosition({ 900.f, 600.f });
	m_recycleBin.setFillColor(sf::Color(150, 0, 0, 180));
	m_recycleBin.setOutlineThickness(2.f);
	m_recycleBin.setOutlineColor(sf::Color::Red);

	m_recycleBinText.setFont(m_font);
	m_recycleBinText.setString("DEL");
	m_recycleBinText.setCharacterSize(20);
	m_recycleBinText.setPosition({ 920.f, 625.f });
	m_recycleBinText.setFillColor(sf::Color::White);
}

std::optional<int> SFMLView::GetSidebarElementAt(Vec2 mousePos, const AlchemyModel& model)
{
	// поправить z-index
	if (mousePos.x > 300)
	{
		return std::nullopt;
	}
	int y = 50;
	for (const auto& el : model.GetElements())
	{
		if (el.isDiscovered)
		{
			if (mousePos.y >= static_cast<float>(y) && mousePos.y <= static_cast<float>(y) + 30)
			{
				return el.id;
			}
			y += 35;
		}
	}
	return std::nullopt;
}

void SFMLView::Render(const AlchemyModel& model)
{
	m_window.clear(sf::Color(40, 40, 40));

    // Рисуем Sidebar
    sf::RectangleShape sidebar({300.f, 700.f});
    sidebar.setFillColor(sf::Color(25, 25, 25));
    m_window.draw(sidebar);

    // 1. Отрисовка списка слева (Sidebar)
    int y = 30;
    for (const auto& el : model.GetElements()) {
        if (el.isDiscovered) {
            // Иконка в сайдбаре
            if (m_elementTextures.contains(el.id)) {
                sf::Sprite icon(m_elementTextures[el.id]);
                // Скейлим под SIDEBAR_ICON_SIZE
                auto texSize = m_elementTextures[el.id].getSize();
                icon.setScale({SIDEBAR_ICON_SIZE / texSize.x, SIDEBAR_ICON_SIZE / texSize.y});
                icon.setPosition({15.f, (float)y});
                m_window.draw(icon);
            }

            sf::Text text(m_font, el.name, 16);
            text.setPosition({55.f, (float)y + 5}); // Текст чуть правее иконки
            text.setFillColor(sf::Color(el.color.r, el.color.g, el.color.b));
            m_window.draw(text);
            y += 40;
        }
    }

    // 2. Отрисовка предметов на поле
    const auto& fieldItems = model.GetFieldItems();
    for (int i = 0; i < (int)fieldItems.size(); ++i) {
        int elId = fieldItems[i].elementId;

        if (m_elementTextures.contains(elId)) {
            sf::Sprite sprite(m_elementTextures[elId]);

            // Расчет скейла: Желаемый_размер / Реальный_размер_текстуры
            auto texSize = m_elementTextures[elId].getSize();
            sprite.setScale({TARGET_SIZE / texSize.x, TARGET_SIZE / texSize.y});

            sprite.setPosition({fieldItems[i].position.x, fieldItems[i].position.y});

            // Если предмет тащат - подсветим его или сделаем прозрачнее
            if (model.GetDraggedIndex() == i) {
                sprite.setColor(sf::Color(255, 255, 255, 180));
            }

            m_window.draw(sprite);
        } else {
            // Если картинка не нашлась - рисуем запасной прямоугольник
            sf::RectangleShape fallback({TARGET_SIZE, TARGET_SIZE});
            fallback.setPosition({fieldItems[i].position.x, fieldItems[i].position.y});
            fallback.setFillColor(sf::Color::Magenta);
            m_window.draw(fallback);
        }
    }

	sf::Text status(m_font, model.GetStatusMessage(), 20);
	status.setPosition({ 320.f, 650.f });
	m_window.draw(status);

	m_window.draw(m_recycleBin);
	m_window.draw(m_recycleBinText);

	m_window.display();
}

bool SFMLView::IsInTrashZone(const Vec2 pos)
{
	return (pos.x >= 900 && pos.x <= 980 && pos.y >= 600 && pos.y <= 680);
}

void SFMLView::LoadTextures(const AlchemyModel& model)
{
	for (const auto& el : model.GetElements())
	{
		std::string path = el.name + ".png";

		if (sf::Texture tex; tex.loadFromFile(path))
		{
			m_elementTextures[el.id] = std::move(tex);
		}
		else
		{
			std::cerr << "Warning: Could not load image: " << path << std::endl;
		}
	}
}