// SDL includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ncurses.h>

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
#include "CPresentation.hpp"
// Filters
#include "CFilterBrightness.hpp"
#include "CFilterContrast.hpp"
#include "CFilterCrop.hpp"
#include "CFilterFlipHorizontal.hpp"
#include "CFilterFlipVertical.hpp"
#include "CFilterInvert.hpp"
#include "CFilterMirrorHorizontal.hpp"
#include "CFilterMirrorVertical.hpp"
#include "CFilterResize.hpp"
#include "CFilterRotateLeft.hpp"
#include "CFilterRotateRight.hpp"

/// @brief Prints the help to this program
/// @param progName The file name of the program
void printHelp(const char *progName) {
	cout << "Usage: " << progName << " [options]\n"
		 << "-i path : defines a path to an image to convert, may be used multiple times\n"
		 << "-cs [characters] [indexes] : defines a character set from which to generate an ascii art, cannot be more than 256 characters long\n"
		 << "-os [value] : selects the type of output to use (only one can be used)\n"
		 << "            - 1 : prints out all the results to the console\n"
		 << "            - 2 [path]: prints each individual result to files\n"
		 << "            - 3 : starts a presentation of the images (space to start/stop, left to go to previous, right to go to next, q to exit)\n"
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

/// @brief Gets the indexes of images from the argument
/// @param arg The pointer to the c string representing the argument
/// @return Vector of indexes
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

/// @brief Adds a filter to images specified by the index vector
/// @param art The array of all images
/// @param filter The filter to add
/// @param indexes The indexes where to add the filter
void addFilterToImages(vector<CArt> &art, const shared_ptr<CFilter> &filter, const vector<size_t> &indexes) {
	for (size_t index : indexes) {
		if (art.size() <= index) {
			throw invalid_argument(string("No image defined for index ") + to_string(index));
		}
		art[index].addFilter(filter);
	}
}

/// @brief Handles the arguments from console
/// @param argc The count of arguments
/// @param argv Array of c strings, which are the arguments
/// @return a shared_ptr to an outputter to use
/// @throws invalid_argument when too few arguments were passed
/// @throws invalid_argument when any argument is wrong
/// @throws invalid_argument when no image was specified in the arguments
/// @throws invalid_argument when no outputter was specified in the arguments
shared_ptr<COutput> handleInput(int argc, char *argv[]) {
	vector<CArt> art;
	shared_ptr<COutput> output = shared_ptr<COutput>(nullptr);

	if (argc == 1) {
		throw invalid_argument("Too few arguments");
	}

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-i") == 0) {
			if (i + 1 >= argc) {
				throw invalid_argument("Too few arguments for -i");
			}
			++i;
			art.emplace_back(argv[i]);
			continue;
		}

		if (strcmp(argv[i], "-cs") == 0) {
			if (i + 2 >= argc) {
				throw invalid_argument("Too few arguments for -cs");
			}

			CCharset cs(argv[i + 1]);
			vector<size_t> indexes = getImageIdx(argv[i + 2]);
			for (size_t index : indexes) {
				if (art.size() <= index) {
					throw invalid_argument(string("No image defined for index ") + to_string(index));
				}
				art[index].setCharset(cs);
			}
			i += 2;
			continue;
		}

		if (strcmp(argv[i], "-os") == 0) {
			if (i + 1 >= argc) {
				throw invalid_argument("Too few arguments for -os");
			}
			++i;
			int arg = atoi(argv[i]);
			switch (arg) {
				case 1: {
					output = CConsole().clone();
					break;
				}
				case 2: {
					if (i + 1 >= argc) {
						throw invalid_argument("Too few arguments for -os 2");
					}
					++i;
					output = CFile(argv[i]).clone();
					break;
				}
				case 3: {
					output = CPresentation().clone();
					break;
				}
				default: {
					throw invalid_argument("Unknown argument for -os");
				}
			}
			continue;
		}

		if (strcmp(argv[i], "-fs") == 0) {
			if (i + 3 >= argc) {
				throw invalid_argument("Too few arguments for -fs");
			}
			addFilterToImages(art, CFilterResize(atoi(argv[i + 1]), atoi(argv[i + 2])).clone(), getImageIdx(argv[i + 3]));
			i += 3;
			continue;
		}

		if (strcmp(argv[i], "-fcr") == 0) {
			if (i + 5 >= argc) {
				throw invalid_argument("Too few arguments for -fcr");
			}
			addFilterToImages(art, CFilterCrop(atoi(argv[i + 1]), atoi(argv[i + 2]), atoi(argv[i + 3]), atoi(argv[i + 4])).clone(), getImageIdx(argv[i + 5]));
			i += 5;
			continue;
		}

		if (strcmp(argv[i], "-fb") == 0) {
			if (i + 2 >= argc) {
				throw invalid_argument("Too few arguments for -fb");
			}
			addFilterToImages(art, CFilterBrightness(atoi(argv[i + 1])).clone(), getImageIdx(argv[i + 2]));
			i += 2;
			continue;
		}

		if (strcmp(argv[i], "-fct") == 0) {
			if (i + 2 >= argc) {
				throw invalid_argument("Too few arguments for -fct");
			}
			addFilterToImages(art, CFilterContrast(atoi(argv[i + 1])).clone(), getImageIdx(argv[i + 2]));
			i += 2;
			continue;
		}

		if (strcmp(argv[i], "-frr") == 0) {
			if (i + 1 >= argc) {
				throw invalid_argument("Too few arguments for -frr");
			}
			++i;
			addFilterToImages(art, CFilterRotateRight().clone(), getImageIdx(argv[i]));
			continue;
		}

		if (strcmp(argv[i], "-frl") == 0) {
			if (i + 1 >= argc) {
				throw invalid_argument("Too few arguments for -frl");
			}
			++i;
			addFilterToImages(art, CFilterRotateLeft().clone(), getImageIdx(argv[i]));
			continue;
		}

		if (strcmp(argv[i], "-fmh") == 0) {
			if (i + 1 >= argc) {
				throw invalid_argument("Too few arguments for -fmh");
			}
			++i;
			addFilterToImages(art, CFilterMirrorHorizontal().clone(), getImageIdx(argv[i]));
			continue;
		}

		if (strcmp(argv[i], "-fmv") == 0) {
			if (i + 1 >= argc) {
				throw invalid_argument("Too few arguments for -fmv");
			}
			++i;
			addFilterToImages(art, CFilterMirrorVertical().clone(), getImageIdx(argv[i]));
			continue;
		}

		if (strcmp(argv[i], "-ffh") == 0) {
			if (i + 1 >= argc) {
				throw invalid_argument("Too few arguments for -ffh");
			}
			++i;
			addFilterToImages(art, CFilterFlipHorizontal().clone(), getImageIdx(argv[i]));
			continue;
		}

		if (strcmp(argv[i], "-ffv") == 0) {
			if (i + 1 >= argc) {
				throw invalid_argument("Too few arguments for -ffv");
			}
			++i;
			addFilterToImages(art, CFilterFlipVertical().clone(), getImageIdx(argv[i]));
			continue;
		}

		if (strcmp(argv[i], "-fi") == 0) {
			if (i + 1 >= argc) {
				throw invalid_argument("Too few arguments for -fi");
			}
			++i;
			addFilterToImages(art, CFilterInvert().clone(), getImageIdx(argv[i]));
			continue;
		}

		throw invalid_argument(string("Unknown argument: ") + argv[i]);
	}

	if (art.size() == 0) {
		throw invalid_argument("No image was specified");
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
