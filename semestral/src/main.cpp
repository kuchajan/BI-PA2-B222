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
