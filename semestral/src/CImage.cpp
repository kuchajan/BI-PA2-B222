#include <iostream>
#include <stdexcept>

#include "CImage.hpp"

using namespace std;

inline bool CImage::checkPos(int x, int y) const {
	return x < m_data->w && y < m_data->h;
}

uint32_t CImage::getPixel(int x, int y) const {
	if (!checkPos(x, y)) {
		throw invalid_argument("CImage::getPixel: The x or y is outside of bounds of image");
	}

	if (SDL_LockSurface(m_data) != 0) {
		throw invalid_argument("CImage::getPixel: Failed to lock surface");
	}

	uint8_t *pixel = (uint8_t *)m_data->pixels + y * m_data->pitch + x * m_data->format->BytesPerPixel;

	SDL_UnlockSurface(m_data);

	uint32_t value;
	switch (m_data->format->BytesPerPixel) {
		case 1: {
			value = *pixel;
			break;
		}

		case 2: {
			value = *(uint16_t *)pixel;
			break;
		}

		case 3: {
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				value = pixel[0] << 16 | pixel[1] << 8 | pixel[2];
			} else {
				value = pixel[0] | pixel[1] << 8 | pixel[2] << 16;
			}
			break;
		}

		case 4: {
			value = *(uint32_t *)pixel;
			break;
		}

		default: {
			throw logic_error("getPixel: Unexpected pixel format");
		}
	}

	return value;
}

// Constructors / destructors
/*CImage::CImage() {
	m_data = nullptr;
}*/

CImage::CImage(const char *filepath) {
	m_data = IMG_Load(filepath);
	if (!m_data) {
		throw logic_error("CImage::CImage: Failed to load image");
	}
}

CImage::CImage(const CImage &src) {
	m_data = nullptr;
	if (SDL_BlitSurface(src.m_data, nullptr, m_data, nullptr)) {
		throw logic_error("CImage::CImage: Failed to copy image data");
	}
}

CImage::~CImage() {
	SDL_FreeSurface(m_data);
	m_data = nullptr;
}

CImage CImage::operator=(CImage src) {
	swap(m_data, src.m_data);
	return *this;
}

// getters
int CImage::getWidth() const {
	return m_data->w;
}

int CImage::getHeight() const {
	return m_data->h;
}

uint8_t CImage::getGrayPixel(int x, int y) const {
	uint32_t pixel = getPixel(x, y);
	uint8_t red = 0, green = 0, blue = 0;
	if (SDL_LockSurface(m_data) != 0) {
		throw invalid_argument("CImage::getGrayPixel: Failed to lock surface");
	}
	SDL_GetRGB(pixel, m_data->format, &red, &green, &blue);
	SDL_UnlockSurface(m_data);
	return (red + green + blue) / 3;
}