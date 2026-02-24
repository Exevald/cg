#pragma once

#include "../dialogs/IFileDialog.h"
#include "../model/ImageModel.h"
#include "../view/ImageView.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

class ImageController
{
public:
	ImageController();
	void Run();

private:
	sf::RenderWindow m_window;
	ImageModel m_model;
	ImageView m_view;
	std::unique_ptr<IFileDialog> m_fileDialog;

	sf::Vector2f m_imageOffset;
	float m_currentScale;

	void HandleEvents();
	void OpenFileDialog();
	void ResetViewTransform();
	void Render();

	static sf::VideoMode GetFullscreenMode();
};