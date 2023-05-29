#include "CFilterResize.hpp"
#include "CRatio.hpp"

using namespace std;

char CFilterResize::average(const CMatrix<char> &input, int fromX, int toX, int fromY, int toY) {
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
	CRatio inverseOld(1, oldDimSize);

	CRatio from(index, newDimSize);
	CRatio to(index + 1, newDimSize);

	return make_pair<int, int>(inverseOld.nearestLowestRoundNumerator(from), inverseOld.nearestHighestRoundNumerator(to));
}

CFilterResize::CFilterResize(int width, int height) : m_newWidth(width), m_newHeight(height) {}