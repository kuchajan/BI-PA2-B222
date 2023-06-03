#include "CFilterFlipHorizontal.hpp"

CMatrix<unsigned char> CFilterFlipHorizontal::apply(const CMatrix<unsigned char> &input) const {
	CMatrix<unsigned char> result(input.getWidth(), input.getHeight());
	for (int x = 0; x < input.getWidth(); ++x) {
		for (int y = 0; y < input.getHeight(); ++y) {
			result.setData(x, input.getHeight() - y - 1, input.getData(x, y));
		}
	}
	return result;
}

std::shared_ptr<CFilter> CFilterFlipHorizontal::clone() const {
	return make_shared<CFilterFlipHorizontal>(*this);
}
