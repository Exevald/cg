#pragma once

#include <cstdint>
#include <vector>

struct ImageData
{
	std::vector<uint8_t> pixels;
	unsigned int width = 0;
	unsigned int height = 0;
	bool isValid = false;
};

class ImageModel
{
public:
	ImageModel() = default;

	[[nodiscard]] const ImageData& getData() const;
	[[nodiscard]] bool HasImage() const;

	void SetImageData(const uint8_t* pixels, unsigned int width, unsigned int height);
	void Clear();

private:
	ImageData m_data;
};