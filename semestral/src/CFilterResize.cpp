#include "CFilterResize.hpp"

using namespace std;

unsigned char CFilterResize::average(const CMatrix<unsigned char> &input, int fromX, int toX, int fromY, int toY) const {
	double res = 0;
	int i = 0;

	for (int x = fromX; x < toX; ++x) {
		for (int y = fromY; y < toY; ++y) {
			res = res * i / (i + 1) + input.getData(x, y) / (i + 1);
			++i;
		}
	}

	return (unsigned char)res;
}

pair<int, int> CFilterResize::getRange(int index, int oldDimSize, int newDimSize) const {
	int tmp = (index + 1) * oldDimSize;
	int resHi = tmp / newDimSize;
	// (resHi * newDimSize == tmp) is another way to say (tmp % newDimSize == 0)
	return make_pair<int, int>((index * oldDimSize) / newDimSize, ((resHi * newDimSize == tmp) ? resHi : (resHi + 1)));
}

CFilterResize::CFilterResize(int width, int height) : m_newWidth(width), m_newHeight(height) {}

CMatrix<unsigned char> CFilterResize::apply(const CMatrix<unsigned char> &input) const {
	CMatrix<unsigned char> res(m_newWidth, m_newHeight);

	for (int x = 0; x < m_newWidth; ++x) {
		for (int y = 0; y < m_newHeight; ++y) {
			pair<int, int> rangeX = getRange(x, input.getWidth(), m_newWidth);
			pair<int, int> rangeY = getRange(y, input.getHeight(), m_newHeight);
			res.setData(x, y, average(input, rangeX.first, rangeX.second, rangeY.first, rangeY.second));
		}
	}

	return res;
}

shared_ptr<CFilter> CFilterResize::clone() const {
	return make_shared<CFilterResize>(*this);
}
