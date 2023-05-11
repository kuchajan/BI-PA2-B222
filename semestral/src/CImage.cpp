#include <iostream>
#include <stdexcept>

#include "CImage.hpp"

using namespace std;

// Constructors / destructors
CImage::CImage() {
	m_data = nullptr;
}

CImage::CImage(const char *filepath) {
	m_data = IMG_Load(filepath);
	if (!m_data) {
		throw logic_error("CImage::CImage: Failed to load image");
	}
}

CImage::CImage(const CImage &src) {
	if (SDL_BlitSurface(src.m_data, nullptr, m_data, nullptr)) {
		throw logic_error("CImage::CImage: Failed to copy image data");
	}
}

CImage::~CImage() {
	SDL_FreeSurface(m_data);
	m_data = nullptr;
}