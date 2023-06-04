#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

using namespace std;
// todo: use SDL_GetError

/// @brief Wrapper around SDL to a given image
class CImage {
private:
	/// @brief Pointer to SDL_Surface containing loaded image
	SDL_Surface *m_data;

	/// @brief Checks whether position is valid
	/// @param x The collumn
	/// @param y The row
	/// @return True if valid, otherwise false
	inline bool checkPos(int x, int y) const;

	/// @brief Gets a value of a pixel from a given position on an image
	/// @param x The x position
	/// @param y The y position
	/// @return The value of a pixel
	/// @throws logic_error when m_data->format->BytesPerPixel is lower than 1 or higher than 4
	/// @throws logic_error when surface locking fails
	/// @throws invalid_argument when position is invalid
	uint32_t getPixel(int x, int y) const;

public:
	/// @brief Empty image
	// CImage(); // maybe don't use?

	/// @brief Uses SDL to load an image
	/// @param filepath Path to the image
	CImage(const char *filepath);

	/// @brief Copies an image from another CImage object
	/// @param src The source CImage
	CImage(const CImage &src);

	~CImage();

	/// @brief Creates a copy of the right hand side and swaps data
	/// @param src The source image to copy
	/// @return Swapped copy of right hand side
	CImage operator=(CImage src);

	/// @brief Gets the width of the loaded image
	/// @return The width
	int getWidth() const;

	/// @brief Gets the height of the loaded image
	/// @return The height
	int getHeight() const;

	/// @brief Gets a gray pixel value from a given position
	/// @param x The collumn in the image
	/// @param y The row in the image
	/// @return The gray pixel
	uint8_t getGrayPixel(int x, int y) const;
};