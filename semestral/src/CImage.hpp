#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

using namespace std;
// todo: use SDL_GetError
class CImage {
private:
	SDL_Surface *m_data;

	inline bool checkPos(const int &x, const int &y) const;
	uint32_t getPixel(const int &x, const int &y);

public:
	CImage();
	CImage(const char *filepath);
	CImage(const CImage &src);
	~CImage();

	int getWidth() const;
	int getHeight() const;
	SDL_PixelFormat *getFormat() const;

	uint8_t getGrayPixel(const int &x, const int &y);

	// todo: overload operator=
};