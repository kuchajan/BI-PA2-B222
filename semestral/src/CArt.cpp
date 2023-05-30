#include "CArt.hpp"

CMatrix<char> CArt::getResult(const CMatrix<unsigned char> &grayScale, const CCharset &charset) const {
	CMatrix<char> result(grayScale.getWidth(), grayScale.getHeight());
	for (int i = 0; i < result.getElementCount(); ++i) {
		result.setData(i, charset.getChar(grayScale.getData(i)));
	}
	return result;
}

CArt::CArt(const CImage &image, const CCharset &charset, const vector<shared_ptr<CFilter>> &filters) : m_result(getResult(getGrayScale(image, filters), charset)) {}