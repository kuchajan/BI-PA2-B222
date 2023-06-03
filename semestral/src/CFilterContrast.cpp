#include "CFilterContrast.hpp"

CFilterContrast::CFilterContrast(int value) {
	if (value < -255 || value > 255) {
		throw invalid_argument("CFilterContrast: value to change contrast by is not within interval <-255,255>");
	}
	m_value = ((value + 255) * 259) / ((259 - value) * 255); //(-255,-1) to (0,1), 1 to 1, (1,255) to (1,~128)
}

CMatrix<unsigned char> CFilterContrast::apply(const CMatrix<unsigned char> &input) const {
	CMatrix<unsigned char> result(input.getWidth(), input.getHeight());
	for (size_t i = 0; i < input.getElementCount(); ++i) {
		int data = m_value * (((int)input.getData(i)) - 128) + 128;
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

std::shared_ptr<CFilter> CFilterContrast::clone() const {
	return make_shared<CFilterContrast>(*this);
}
