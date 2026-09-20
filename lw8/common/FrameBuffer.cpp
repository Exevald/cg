#include "FrameBuffer.h"

#include <algorithm>

FrameBuffer::FrameBuffer(const unsigned width, const unsigned height)
{
	Resize(width, height);
}

void FrameBuffer::Resize(const unsigned width, const unsigned height)
{
	m_width = width;
	m_height = height;
	m_pixels.assign(static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4, 0);
}

void FrameBuffer::Clear(const glm::vec3& color)
{
	const std::uint8_t r = ToByte(color.r);
	const std::uint8_t g = ToByte(color.g);
	const std::uint8_t b = ToByte(color.b);
	for (std::size_t i = 0; i < m_pixels.size(); i += 4)
	{
		m_pixels[i + 0] = r;
		m_pixels[i + 1] = g;
		m_pixels[i + 2] = b;
		m_pixels[i + 3] = 255;
	}
}

void FrameBuffer::SetPixel(const unsigned x, const unsigned y, const glm::vec3& color) noexcept
{
	const std::size_t index = (static_cast<std::size_t>(y) * m_width + x) * 4;
	m_pixels[index + 0] = ToByte(color.r);
	m_pixels[index + 1] = ToByte(color.g);
	m_pixels[index + 2] = ToByte(color.b);
	m_pixels[index + 3] = 255;
}

unsigned FrameBuffer::GetWidth() const noexcept
{
	return m_width;
}

unsigned FrameBuffer::GetHeight() const noexcept
{
	return m_height;
}

const std::uint8_t* FrameBuffer::Data() const noexcept
{
	return m_pixels.data();
}

std::uint8_t* FrameBuffer::Data() noexcept
{
	return m_pixels.data();
}

std::uint8_t FrameBuffer::ToByte(const float value) noexcept
{
	return static_cast<std::uint8_t>(std::clamp(value, 0.0f, 1.0f) * 255.0f + 0.5f);
}