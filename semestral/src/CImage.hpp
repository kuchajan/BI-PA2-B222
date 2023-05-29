#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

using namespace std;
// todo: use SDL_GetError

///@brief Wrapper around SDL
class CImage {
private:
	SDL_Surface *m_data;

	inline bool checkPos(const int &x, const int &y) const;
	uint32_t getPixel(const int &x, const int &y) const;

public:
	CImage();
	CImage(const char *filepath);
	CImage(const CImage &src);
	~CImage();

	CImage operator=(CImage src);

	int getWidth() const;
	int getHeight() const;
	SDL_PixelFormat *getFormat() const;

	uint8_t getGrayPixel(const int &x, const int &y) const;
};