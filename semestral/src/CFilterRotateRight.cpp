#include "CFilterRotateRight.hpp"

CMatrix<unsigned char> CFilterRotateRight::apply(const CMatrix<unsigned char> &input) const {
	CMatrix<unsigned char> result(input.getHeight(), input.getWidth());
	for (int x = 0; x < input.getWidth(); ++x) {
		for (int y = 0; y < input.getHeight(); ++y) {
			result.setData(input.getHeight() - y - 1, x, input.getData(x, y));
		}
	}
	return result;
}

std::shared_ptr<CFilter> CFilterRotateRight::clone() const {
	return make_shared<CFilterRotateRight>(*this);
}
