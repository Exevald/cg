#pragma once

#include "../model/AlchemyModel.h"
#include "IAlchemyView.h"

#include <SFML/Graphics.hpp>
#include <optional>

class SFMLView : public IAlchemyView
{

public:
	SFMLView(const AlchemyModel& model);

	void Render(const AlchemyModel& model) override;
	bool IsOpen() const override { return m_window.isOpen(); }

	sf::RenderWindow& GetWindow() { return m_window; }
	static std::optional<int> GetSidebarElementAt(Vec2 mousePos, const AlchemyModel& model);
	static bool IsInTrashZone(Vec2 pos);

private:
	void LoadTextures(const AlchemyModel& model); // Метод для предварительной загрузки

	std::map<int, sf::Texture> m_elementTextures; // Кэш текстур: ID -> Texture
	const float TARGET_SIZE = 50.0f; // Желаемый размер иконки на поле
	const float SIDEBAR_ICON_SIZE = 30.0f; // Размер иконки в списке слева

	sf::RenderWindow m_window;
	sf::Font m_font;
	sf::RectangleShape m_sidebar;
	sf::RectangleShape m_recycleBin;
	sf::Text m_recycleBinText;
};