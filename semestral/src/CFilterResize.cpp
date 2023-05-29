#include "CFilterResize.hpp"

using namespace std;

char CFilterResize::average(const CMatrix<char> &input, int fromX, int toX, int fromY, int toY) const {
	double res = 0;
	int i = 0;

	for (int x = fromX; x < toX; ++x) {
		for (int y = fromY; x < toY; ++y) {
			res = res * i / (i + 1) + input.getData(x, y) / (i + 1);
			++i;
		}
	}

	return (char)res;
}

pair<int, int> CFilterResize::getRange(int index, int oldDimSize, int newDimSize) const {
	int tmp = (index + 1) * oldDimSize;
	int resHi = tmp / newDimSize;
	// (resHi * newDimSize == tmp) is another way to say (tmp % newDimSize == 0)
	return make_pair<int, int>((index * oldDimSize) / newDimSize, ((resHi * newDimSize == tmp) ? resHi : (resHi + 1)));
}

CFilterResize::CFilterResize(int width, int height) : m_newWidth(width), m_newHeight(height) {}