// SDL includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

#include "CArt.hpp"
#include "CCharset.hpp"
#include "CFilter.hpp"
#include "CImage.hpp"
#include "CMatrix.hpp"
#include "COutput.hpp"
// Outputs
#include "CConsole.hpp"
#include "CFile.hpp"
// Filters
#include "CFilterResize.hpp"

void printHelp(const char *progName) {
	cout << "Usage: " << progName << " [options]\n"
		 << "-i path : defines a path to an image to convert, may be used multiple times\n"
		 << "-cs [characters] [indexes] : defines a character set from which to generate an ascii art, cannot be more than 256 characters long\n"
		 << "-os [value] : selects the type of output to use (only one can be used)\n"
		 << "            - 1 : prints out all the results to the console\n"
		 << "            - 2 [path]: prints each individual result to files\n"
		 << "-fs [width] [height] [indexes] : resizes images to given width and height\n"
		 << "-fcr [fromX] [fromY] [width] [height] [indexes] : crops images\n"
		 << "-fb [value] [indexes] : changes the brightness by given value\n"
		 << "-fct [value] [indexes] : changes the contrast by given value\n"
		 << "-frr [indexes] : rotates the image by 90 degrees to the right\n"
		 << "-frl [indexes] : rotates the image by 90 degrees to the left\n"
		 << "-fmh [indexes] : mirrors the image horizontally\n"
		 << "-fmv [indexes] : mirrors the image vertically\n"
		 << "-ffh [indexes] : flips the image horizontally\n"
		 << "-ffv [indexes] : flips the image vertically\n"
		 << "-fi [indexes] : inverts the image\n"
		 << endl;
}

vector<size_t> getImageIdx(char *arg) {
	vector<size_t> imageIdx;
	istringstream iss(arg);

	char ch = ',';
	while (!iss.eof()) {
		if (iss.fail() || ch != ',') {
			throw invalid_argument("Bad input of image indexes");
		}
		size_t n;
		iss >> n;
		if (iss.bad()) {
			throw invalid_argument("Bad input of image indexes");
		}
		imageIdx.push_back(n);
		iss >> ch;
	}

	return imageIdx;
}

shared_ptr<COutput> handleInput(int argc, char *argv[]) {
	vector<CArt> art;
	shared_ptr<COutput> output = shared_ptr<COutput>(nullptr);

	if (argc == 1) {
		throw invalid_argument("Too few arguments");
	}

	for (int i = 1; i < argc; ++i) {
		// todo: iterate through all arguments and their sub arguments
		if (strcmp(argv[i], "-i") == 0) {
			if (i + 1 >= argc) {
				throw invalid_argument("Too few arguments for -i");
			}
			++i;
			art.emplace_back(argv[i]);
			continue;
		}

		if (strcmp(argv[i], "-os") == 0) {
			if (i + 1 >= argc) {
				throw invalid_argument("Too few arguments for -os");
			}
			++i;
			// todo: parse next argument to choose outputter
			if (strcmp(argv[i], "1") == 0) {
				output = CConsole().clone();
				continue;
			}
			if (strcmp(argv[i], "2") == 0) {
				if (i + 1 >= argc) {
					throw invalid_argument("Too few arguments for -os 2");
				}
				++i;
				output = CFile(argv[i]).clone();
				continue;
			}

			throw invalid_argument("Unknown argument for -os");
		}
		throw invalid_argument(string("Unknown argument: ") + argv[i]);
	}

	if (!output) {
		throw invalid_argument("No outputter specified");
	}
	output->addArts(art);
	return output;
}

int main(int argc, char *argv[]) {
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) == 0) {
		throw logic_error("IMG_Init fail");
	}
	shared_ptr<COutput> output;
	try {
		output = handleInput(argc, argv);
	} catch (const exception &e) {
		IMG_Quit();
		cerr << e.what() << '\n';
		printHelp(argv[0]);
		return 0;
	}

	output->output();

	IMG_Quit();
	return 0;
}
