#include "TextureLoader.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

namespace
{

template <typename T>
T Read(std::ifstream& input)
{
	T value{};
	input.read(reinterpret_cast<char*>(&value), sizeof(value));
	return value;
}

} // namespace

GLuint LoadTextureFromBmp(const std::string& path)
{
	std::ifstream input(path, std::ios::binary);
	if (!input)
	{
		throw std::runtime_error("Unable to open file");
	}

	if (const auto signature = Read<std::uint16_t>(input); signature != 0x4D42)
	{
		throw std::runtime_error("Unsupported BMP texture signature");
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

	if (const auto compression = Read<std::uint32_t>(input); dibHeaderSize < 40
		|| planes != 1
		|| signedWidth <= 0
		|| signedHeight == 0
		|| (bitsPerPixel != 24 && bitsPerPixel != 32)
		|| (compression != 0 && compression != 3))
	{
		throw std::runtime_error("Unsupported BMP texture signature");
	}

	const auto width = static_cast<std::uint32_t>(signedWidth);
	const auto height = static_cast<std::uint32_t>(std::abs(signedHeight));
	const bool topDown = signedHeight < 0;
	const std::size_t sourceBytesPerPixel = bitsPerPixel / 8;
	const std::size_t sourceStride = (static_cast<std::size_t>(width) * sourceBytesPerPixel + 3) / 4 * 4;
	std::vector<unsigned char> source(sourceStride * height);

	input.seekg(pixelOffset, std::ios::beg);
	input.read(reinterpret_cast<char*>(source.data()), static_cast<std::streamsize>(source.size()));
	if (!input)
	{
		throw std::runtime_error("Unable to open file");
	}

	std::vector<unsigned char> pixels(static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4);
	for (std::uint32_t y = 0; y < height; ++y)
	{
		const std::uint32_t sourceY = topDown ? y : height - 1 - y;
		const auto* sourceRow = source.data() + static_cast<std::size_t>(sourceY) * sourceStride;
		auto* targetRow = pixels.data()
			+ static_cast<std::size_t>(y) * static_cast<std::size_t>(width) * 4;
		for (std::uint32_t x = 0; x < width; ++x)
		{
			const auto* sourcePixel = sourceRow
				+ static_cast<std::size_t>(x) * sourceBytesPerPixel;
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
	glGenerateMipmap(GL_TEXTURE_2D);
	return textureId;
}