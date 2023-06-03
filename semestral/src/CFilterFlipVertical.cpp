#include "CFilterFlipVertical.hpp"

CMatrix<unsigned char> CFilterFlipVertical::apply(const CMatrix<unsigned char> &input) const {
	CMatrix<unsigned char> result(input.getWidth(), input.getHeight());
	for (int x = 0; x < input.getWidth(); ++x) {
		for (int y = 0; y < input.getHeight(); ++y) {
			result.setData(input.getWidth() - x - 1, y, input.getData(x, y));
		}
	}
	return result;
}

std::shared_ptr<CFilter> CFilterFlipVertical::clone() const {
	return make_shared<CFilterFlipVertical>(*this);
}
