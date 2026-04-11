#include "TextureBuilder.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

namespace
{

constexpr int TEXTURE_SIZE = 64;

GLuint CreateTexture(const std::array<unsigned char, TEXTURE_SIZE * TEXTURE_SIZE * 3>& pixels)
{
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		TEXTURE_SIZE,
		TEXTURE_SIZE,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		pixels.data());
	return textureId;
}

void PutPixel(
	std::array<unsigned char, TEXTURE_SIZE * TEXTURE_SIZE * 3>& pixels,
	const int x,
	const int y,
	const unsigned char r,
	const unsigned char g,
	const unsigned char b)
{
	const int index = (y * TEXTURE_SIZE + x) * 3;
	pixels[index] = r;
	pixels[index + 1] = g;
	pixels[index + 2] = b;
}

template <typename T>
T Read(std::ifstream& input)
{
	T value{};
	input.read(reinterpret_cast<char*>(&value), sizeof(value));
	return value;
}

} // namespace

GLuint TextureBuilder::CreateTextureFromFile(const std::string& path)
{
	std::ifstream input(path, std::ios::binary);
	if (!input)
	{
		std::cerr << "Texture load failed: " << path << std::endl;
		return 0;
	}

	if (const auto signature = Read<std::uint16_t>(input);
		signature != 0x4D42)
	{
		std::cerr << "Texture load failed: " << path << std::endl;
		return 0;
	}

	Read<std::uint32_t>(input);
	Read<std::uint16_t>(input);
	Read<std::uint16_t>(input);
	const auto pixelOffset = Read<std::uint32_t>(input);
	const auto dibHeaderSize = Read<std::uint32_t>(input);
	const auto signedWidth = Read<std::int32_t>(input);
	const auto signedHeight = Read<std::int32_t>(input);
	const auto planes = Read<std::uint16_t>(input);
	const auto bitsPerPixel = Read<std::uint16_t>(input);

	if (const auto compression = Read<std::uint32_t>(input);
		planes != 1
		|| signedWidth <= 0
		|| signedHeight == 0
		|| (bitsPerPixel != 24 && bitsPerPixel != 32)
		|| (compression != 0 && compression != 3))
	{
		std::cerr << "Unsupported BMP texture: " << path << std::endl;
		return 0;
	}

	const auto width = static_cast<std::uint32_t>(signedWidth);
	const auto height = static_cast<std::uint32_t>(std::abs(signedHeight));
	const bool topDown = signedHeight < 0;
	const std::size_t sourceBytesPerPixel = bitsPerPixel / 8;
	const std::size_t sourceStride = ((static_cast<std::size_t>(width) * sourceBytesPerPixel + 3) / 4) * 4;
	std::vector<unsigned char> source(sourceStride * height);

	input.seekg(pixelOffset, std::ios::beg);
	input.read(reinterpret_cast<char*>(source.data()), static_cast<std::streamsize>(source.size()));
	if (!input)
	{
		std::cerr << "Texture load failed: " << path << std::endl;
		return 0;
	}

	if (dibHeaderSize < 40)
	{
		std::cerr << "Unsupported BMP texture: " << path << std::endl;
		return 0;
	}

	std::vector<unsigned char> pixels(static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4);
	for (std::uint32_t y = 0; y < height; ++y)
	{
		const std::uint32_t sourceY = topDown ? (height - 1 - y) : y;
		const auto* sourceRow = source.data() + static_cast<std::size_t>(sourceY) * sourceStride;
		auto* targetRow = pixels.data() + static_cast<std::size_t>(y) * static_cast<std::size_t>(width) * 4;
		for (std::uint32_t x = 0; x < width; ++x)
		{
			const auto* sourcePixel = sourceRow + static_cast<std::size_t>(x) * sourceBytesPerPixel;
			auto* targetPixel = targetRow + static_cast<std::size_t>(x) * 4;
			targetPixel[0] = sourcePixel[2];
			targetPixel[1] = sourcePixel[1];
			targetPixel[2] = sourcePixel[0];
			targetPixel[3] = bitsPerPixel == 32 ? sourcePixel[3] : 255;
		}
	}

	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		static_cast<GLsizei>(width),
		static_cast<GLsizei>(height),
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pixels.data());
	return textureId;
}

GLuint TextureBuilder::CreateCheckerTexture(
	const unsigned char r1,
	const unsigned char g1,
	const unsigned char b1,
	const unsigned char r2,
	const unsigned char g2,
	const unsigned char b2)
{
	std::array<unsigned char, TEXTURE_SIZE * TEXTURE_SIZE * 3> pixels{};
	for (int y = 0; y < TEXTURE_SIZE; ++y)
	{
		for (int x = 0; x < TEXTURE_SIZE; ++x)
		{
			const bool first = ((x / 8) + (y / 8)) % 2 == 0;
			PutPixel(pixels, x, y, first ? r1 : r2, first ? g1 : g2, first ? b1 : b2);
		}
	}
	return CreateTexture(pixels);
}

GLuint TextureBuilder::CreateStripeTexture(
	const unsigned char r1,
	const unsigned char g1,
	const unsigned char b1,
	const unsigned char r2,
	const unsigned char g2,
	const unsigned char b2)
{
	std::array<unsigned char, TEXTURE_SIZE * TEXTURE_SIZE * 3> pixels{};
	for (int y = 0; y < TEXTURE_SIZE; ++y)
	{
		for (int x = 0; x < TEXTURE_SIZE; ++x)
		{
			const bool first = ((x + y) / 9) % 2 == 0;
			PutPixel(pixels, x, y, first ? r1 : r2, first ? g1 : g2, first ? b1 : b2);
		}
	}
	return CreateTexture(pixels);
}

GLuint TextureBuilder::CreateRadialTexture(
	const unsigned char r1,
	const unsigned char g1,
	const unsigned char b1,
	const unsigned char r2,
	const unsigned char g2,
	const unsigned char b2)
{
	std::array<unsigned char, TEXTURE_SIZE * TEXTURE_SIZE * 3> pixels{};
	for (int y = 0; y < TEXTURE_SIZE; ++y)
	{
		for (int x = 0; x < TEXTURE_SIZE; ++x)
		{
			const float dx = static_cast<float>(x) - TEXTURE_SIZE * 0.5f;
			const float dy = static_cast<float>(y) - TEXTURE_SIZE * 0.5f;
			const float distance = std::sqrt(dx * dx + dy * dy) / (TEXTURE_SIZE * 0.7f);
			const float t = std::min(1.0f, distance);
			const auto mix = [t](const unsigned char a, const unsigned char b) {
				return static_cast<unsigned char>(static_cast<float>(a) * (1.0f - t) + static_cast<float>(b) * t);
			};
			PutPixel(pixels, x, y, mix(r1, r2), mix(g1, g2), mix(b1, b2));
		}
	}
	return CreateTexture(pixels);
}
