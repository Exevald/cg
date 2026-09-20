#pragma once

#include <cstdint>
#include <vector>

#include <glm/vec3.hpp>

class FrameBuffer
{
public:
	FrameBuffer() = default;
	FrameBuffer(unsigned width, unsigned height);

	void Resize(unsigned width, unsigned height);
	void Clear(const glm::vec3& color = glm::vec3(0.0f));
	void SetPixel(unsigned x, unsigned y, const glm::vec3& color) noexcept;

	[[nodiscard]] unsigned GetWidth() const noexcept;
	[[nodiscard]] unsigned GetHeight() const noexcept;
	[[nodiscard]] const std::uint8_t* Data() const noexcept;
	[[nodiscard]] std::uint8_t* Data() noexcept;

private:
	static std::uint8_t ToByte(float value) noexcept;

	std::vector<std::uint8_t> m_pixels;
	unsigned m_width = 0;
	unsigned m_height = 0;
};
