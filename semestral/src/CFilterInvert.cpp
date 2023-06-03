#include "CFilterInvert.hpp"

CMatrix<unsigned char> CFilterInvert::apply(const CMatrix<unsigned char> &input) const {
	CMatrix<unsigned char> result(input.getWidth(), input.getHeight());
	for (size_t i = 0; i < input.getElementCount(); ++i) {
		result.setData(i, 255 - input.getData(i));
	}
	return result;
}

std::shared_ptr<CFilter> CFilterInvert::clone() const {
	return make_shared<CFilterInvert>(*this);
}
