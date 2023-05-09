// SDL includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <stdexcept>

using namespace std;

#include "CCharset.hpp"
#include "CMatrix.hpp"

/// @brief Gets a value of a pixel from a given position on a surface
/// @param surface The surface, which should contain an image
/// @param x The x position
/// @param y The y position
/// @return The value of a pixel
/// @exception Throws a logic_error when surface->format->BytesPerPixel is not 1 nor 2 nor 3 nor 4
/// @exception Throws an invalid_argument when x>= surface->w or y >= surface->h
uint32_t getPixel(SDL_Surface *surface, int x, int y) {
	if (x >= surface->w || y >= surface->h) {
		throw invalid_argument("getPixel: The x or y is outside of bounds of surface");
	}

	uint8_t *pixel = (uint8_t *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
	uint32_t value;

	switch (surface->format->BytesPerPixel) {
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

CMatrix<uint8_t> getGrayScale(const char *filepath) {
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) == 0) {
		throw logic_error("getImageData: IMG_Init fail");
	}

	SDL_Surface *image = IMG_Load(filepath);

	if (!image) {
		throw logic_error("getImageData: Failed to load image");
	}

	SDL_LockSurface(image);

	CMatrix<uint8_t> grayScaleData(image->w, image->h);

	for (int y = 0; y < image->h; ++y) {
		for (int x = 0; x < image->w; ++x) {
			uint32_t pixel = getPixel(image, x, y);
			uint8_t red, green, blue;
			SDL_GetRGB(pixel, image->format, &red, &green, &blue);
			// cout << "Writing to [x,y] = [" << x << "," << y << "]" << endl;
			grayScaleData.setData(x, y, (red + green + blue) / 3);
		}
	}

	SDL_UnlockSurface(image);
	SDL_FreeSurface(image);
	IMG_Quit();

	return grayScaleData;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "Usage: " << argv[0] << " file" << endl;
		return 0;
	}

	CMatrix<uint8_t> gs = getGrayScale(argv[1]);
	CCharset charset("MNFV$I*:.");

	for (int y = 0; y < gs.getHeight(); ++y) {
		for (int x = 0; x < gs.getWidth(); ++x) {
			cout << charset.getChar(gs.getData(x, y));
		}
		cout << '\n';
	}

	return 0;
}