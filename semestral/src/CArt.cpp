#include "CArt.hpp"

CMatrix<unsigned char> CArt::getGrayScale() const {
	CImage image(m_path);
	CMatrix<unsigned char> grayScale(image.getWidth(), image.getHeight());

	for (int x = 0; x < image.getWidth(); ++x) {
		for (int y = 0; y < image.getHeight(); ++y) {
			grayScale.setData(x, y, image.getGrayPixel(x, y));
		}
	}

	for (shared_ptr<CFilter> filter : m_filters) {
		grayScale = filter->apply(grayScale);
	}

	return grayScale;
}

CMatrix<char> CArt::getResult() const {
	CMatrix<unsigned char> grayScale = getGrayScale();
	CMatrix<char> result(grayScale.getWidth(), grayScale.getHeight());
	for (size_t i = 0; i < result.getElementCount(); ++i) {
		result.setData(i, m_charset.getChar(grayScale.getData(i)));
	}
	return result;
}

CArt::CArt(char *path) : m_path(path), m_charset(" .:*I$VFNM") {}

void CArt::setCharset(CCharset cs) {
	swap(m_charset, cs);
}

void CArt::addFilter(shared_ptr<CFilter> filter) {
	m_filters.push_back(filter);
}

ostream &operator<<(ostream &os, const CArt &art) {
	CMatrix<char> result = art.getResult();
	for (int y = 0; y < result.getHeight(); ++y) {
		for (int x = 0; x < result.getWidth(); ++x) {
			os << result.getData(x, y);
		}
		os << "\n";
	}
	return os;
}
