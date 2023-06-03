// SDL includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

#include "CArt.hpp"
#include "CCharset.hpp"
#include "CFilter.hpp"
#include "CFilterResize.hpp"
#include "CImage.hpp"
#include "CMatrix.hpp"

// Common charset: "MNFV$I*:."

void printHelp(const char *progName) {
	cout << "Usage: " << progName << " [options]\n"
		 << "-i path : defines a path to an image to convert, may be used multiple times\n"
		 << "-cs [characters] : defines a character set from which to generate an ascii art, cannot be more than 255 characters long\n"
		 << "-os [value] : selects the type of output to use (only one can be used)\n"
		 << "            - 1 : prints out all the results to the console and exits\n"
		 << "            - 2 : prints each individual result to files\n"
		 << "-frs : resets filters\n"
		 << "-fs [width] [height] : resizes an image to given width and height\n"
		 << "-fcr [fromX] [fromY] [toX] [toY] : crops an image\n"
		 << "-fb [value] : changes the brightness by given value\n"
		 << "-fct [value] : changes the contrast by given value\n"
		 << "-frr : rotates the image by 90 degrees to the right\n"
		 << "-frl : rotates the image by 90 degrees to the left\n"
		 << "-fmh : mirrors the image horizontally\n"
		 << "-fmv : mirrors the image vertically\n"
		 << "-ffh : flips the image horizontally\n"
		 << "-ffv : flips the image vertically\n"
		 << "-fi : inverts the image\n"
		 << endl;
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		cout << "Usage: " << argv[0] << " file charset" << endl;
		return 0;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) == 0) {
		throw logic_error("getImageData: IMG_Init fail");
	}

	CImage image(argv[1]);
	CCharset charset(argv[2]);
	vector<shared_ptr<CFilter>> filters;
	CFilterResize tmpFilt(32, 16);
	filters.push_back(tmpFilt.clone());

	CArt art(image, charset, filters);

	cout << art;

	IMG_Quit();
	return 0;
}
