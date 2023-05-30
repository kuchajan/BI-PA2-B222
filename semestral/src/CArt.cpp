#include "CArt.hpp"

CMatrix<unsigned char> CArt::getGrayScale(const CImage &image, const vector<shared_ptr<CFilter>> &filters) const {
	CMatrix<unsigned char> grayScale(image.getWidth(), image.getHeight());

	for (int x = 0; x < image.getWidth(); ++x) {
		for (int y = 0; y < image.getHeight(); ++y) {
			grayScale.setData(x, y, image.getGrayPixel(x, y));
		}
	}

	for (shared_ptr<CFilter> filter : filters) {
		grayScale = filter->apply(grayScale);
	}

	return grayScale;
}

CMatrix<char> CArt::getResult(const CMatrix<unsigned char> &grayScale, const CCharset &charset) const {
	CMatrix<char> result(grayScale.getWidth(), grayScale.getHeight());
	for (int i = 0; i < result.getElementCount(); ++i) {
		result.setData(i, charset.getChar(grayScale.getData(i)));
	}
	return result;
}

CArt::CArt(const CImage &image, const CCharset &charset, const vector<shared_ptr<CFilter>> &filters) : m_result(getResult(getGrayScale(image, filters), charset)) {}

ostream &operator<<(ostream &os, const CArt &art) {
	for (int y = 0; y < art.m_result.getHeight(); ++y) {
		for (int x = 0; x < art.m_result.getWidth(); ++x) {
			os << art.m_result.getData(x, y);
		}
		os << "\n";
	}
	return os;
}
