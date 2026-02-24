#include "ImageModel.h"

const ImageData& ImageModel::getData() const
{
	return m_data;
}

bool ImageModel::HasImage() const
{
	return m_data.isValid;
}

void ImageModel::SetImageData(const uint8_t* pixels, unsigned int width, unsigned int height)
{
	if (!pixels || width == 0 || height == 0)
	{
		return;
	}

	m_data.width = width;
	m_data.height = height;
	m_data.pixels.assign(pixels, pixels + (width * height * 4));
	m_data.isValid = true;
}

void ImageModel::Clear()
{
	m_data = {};
}