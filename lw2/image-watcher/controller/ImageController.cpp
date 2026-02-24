#include "ImageController.h"
#include "../dialogs/MacFileDialog.h"

#include <iostream>
#include <memory>

namespace
{
std::unique_ptr<IFileDialog> CreateFileDialog()
{
	return std::make_unique<MacFileDialog>();
}
} // namespace

ImageController::ImageController()
	: m_window(GetFullscreenMode(), "Image Viewer")
	, m_currentScale(1.0f)
{
	m_window.setFramerateLimit(60);
	m_window.setMouseCursorVisible(true);
}

sf::VideoMode ImageController::GetFullscreenMode()
{
	auto modes = sf::VideoMode::getFullscreenModes();
	if (!modes.empty())
	{
		return modes.front();
	}
	return sf::VideoMode::getDesktopMode();
}

void ImageController::Run()
{
	while (m_window.isOpen())
	{
		HandleEvents();
		Render();
	}
}

void ImageController::HandleEvents()
{
	while (std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
			return;
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->code == sf::Keyboard::Key::Escape)
			{
				m_window.close();
				return;
			}
			if (keyPressed->code == sf::Keyboard::Key::O)
			{
				OpenFileDialog();
			}
		}
	}
}

void ImageController::OpenFileDialog()
{
	if (!m_fileDialog)
	{
		m_fileDialog = CreateFileDialog();
	}

	std::string path = m_fileDialog->ShowOpenDialog();
	if (path.empty())
	{
		return;
	}

	sf::Image sfImage;
	if (!sfImage.loadFromFile(path))
	{
		std::cerr << "Failed to load image: " << path << "\n";
		return;
	}

	const uint8_t* pixels = sfImage.getPixelsPtr();
	if (!pixels)
	{
		std::cerr << "No pixel data in image\n";
		return;
	}

	m_model.SetImageData(pixels, sfImage.getSize().x, sfImage.getSize().y);
	ResetViewTransform();
}

void ImageController::ResetViewTransform()
{
	if (!m_model.HasImage())
	{
		return;
	}

	auto winW = static_cast<float>(m_window.getSize().x);
	auto winH = static_cast<float>(m_window.getSize().y);
	auto imgW = static_cast<float>(m_model.getData().width);
	auto imgH = static_cast<float>(m_model.getData().height);

	float scaleW = (winW * 0.9f) / imgW;
	float scaleH = (winH * 0.9f) / imgH;
	m_currentScale = std::min(scaleW, scaleH);

	if (m_currentScale > 1.0f)
	{
		m_currentScale = 1.0f;
	}

	m_imageOffset = { 0.f, 0.f };
}

void ImageController::Render()
{
	m_window.clear(sf::Color::White);

	if (m_model.HasImage())
	{
		const auto& data = m_model.getData();
		float winW = static_cast<float>(m_window.getSize().x);
		float winH = static_cast<float>(m_window.getSize().y);

		sf::Vector2f centerPos(winW / 2.f, winH / 2.f);
		sf::Vector2f finalPos = centerPos + m_imageOffset;

		float scaledW = static_cast<float>(data.width) * m_currentScale;
		float scaledH = static_cast<float>(data.height) * m_currentScale;

		sf::Vector2f drawPos(
			finalPos.x - scaledW / 2.f,
			finalPos.y - scaledH / 2.f);

		m_view.SetTransform(drawPos, m_currentScale);
		m_view.UpdateTexture(m_window, data);
	}

	m_view.Draw(m_window);
	m_window.display();
}