#include "CFilterRotateLeft.hpp"

CMatrix<unsigned char> CFilterRotateLeft::apply(const CMatrix<unsigned char> &input) const {
	CMatrix<unsigned char> result(input.getHeight(), input.getWidth());
	for (int x = 0; x < input.getWidth(); ++x) {
		for (int y = 0; y < input.getHeight(); ++y) {
			result.setData(y, input.getWidth() - x - 1, input.getData(x, y));
		}
	}
	return result;
}

std::shared_ptr<CFilter> CFilterRotateLeft::clone() const {
	return make_shared<CFilterRotateLeft>(*this);
}
