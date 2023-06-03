#include "CFilterBrightness.hpp"

CFilterBrightness::CFilterBrightness(int value) : m_value(value) {
	if (m_value < -255 || m_value > 255) {
		throw invalid_argument("CFilterBrightness: value to change brightness by is not within interval <-255,255>");
	}
}

CMatrix<unsigned char> CFilterBrightness::apply(const CMatrix<unsigned char> &input) const {
	CMatrix<unsigned char> result(input.getWidth(), input.getHeight());
	for (size_t i = 0; i < input.getElementCount(); ++i) {
		int data = input.getData(i) + m_value;
		if (data < 0) {
			data = 0;
		}
		if (data > 255) {
			data = 255;
		}

		result.setData(i, (unsigned char)data);
	}
	return result;
}

std::shared_ptr<CFilter> CFilterBrightness::clone() const {
	return make_shared<CFilterBrightness>(*this);
}