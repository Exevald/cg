#include "TextureLoader.h"

#include <SFML/Graphics/Image.hpp>
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>

GLuint LoadTextureFromFile(const std::string& path, int& width, int& height)
{
	std::ifstream input(path, std::ios::binary);
	if (!input)
	{
		throw std::runtime_error("Unable to open file: " + path);
	}

	std::vector<char> fileData(
		(std::istreambuf_iterator<char>(input)),
		std::istreambuf_iterator<char>());

	sf::Image image;
	if (!image.loadFromMemory(fileData.data(), fileData.size()))
	{
		throw std::runtime_error("Unable to load image: " + path);
	}

	const auto size = image.getSize();
	if (size.x == 0 || size.y == 0)
	{
		throw std::runtime_error("Empty image: " + path);
	}

	width = static_cast<int>(size.x);
	height = static_cast<int>(size.y);

	const auto* source = image.getPixelsPtr();
	std::vector<unsigned char> pixels(
		static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4);
	const std::size_t rowSize = static_cast<std::size_t>(width) * 4;
	for (int y = 0; y < height; ++y)
	{
		const std::size_t sourceRow = static_cast<std::size_t>(y) * rowSize;
		const std::size_t targetRow = static_cast<std::size_t>(height - 1 - y) * rowSize;
		std::copy_n(source + sourceRow, rowSize, pixels.begin() + static_cast<std::ptrdiff_t>(targetRow));
	}

	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pixels.data());

	return textureId;
}
