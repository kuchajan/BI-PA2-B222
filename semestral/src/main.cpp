// SDL includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <stdexcept>

using namespace std;

#include "CCharset.hpp"
#include "CImage.hpp"
#include "CMatrix.hpp"

CMatrix<uint8_t> getGrayScale(const char *filepath) {
	CImage image(filepath);
	CMatrix<uint8_t> grayScaleData(image.getWidth(), image.getHeight());

	for (int y = 0; y < image.getHeight(); ++y) {
		for (int x = 0; x < image.getWidth(); ++x) {
			grayScaleData.setData(x, y, image.getGrayPixel(x, y));
		}
	}

	return grayScaleData;
}

// Common charset: "MNFV$I*:."

int main(int argc, char *argv[]) {
	if (argc < 3) {
		cout << "Usage: " << argv[0] << " file charset" << endl;
		return 0;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) == 0) {
		throw logic_error("getImageData: IMG_Init fail");
	}

	CMatrix<uint8_t> gs = getGrayScale(argv[1]);
	CCharset charset(argv[2]);

	for (int y = 0; y < gs.getHeight(); ++y) {
		for (int x = 0; x < gs.getWidth(); ++x) {
			cout << charset.getChar(gs.getData(x, y));
		}
		cout << '\n';
	}

	IMG_Quit();
	return 0;
}