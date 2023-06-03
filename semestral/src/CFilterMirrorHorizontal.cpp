#include "CFilterMirrorHorizontal.hpp"

CMatrix<unsigned char> CFilterMirrorHorizontal::apply(const CMatrix<unsigned char> &input) const {
	CMatrix<unsigned char> result(input.getWidth(), input.getHeight());
	for (int x = 0; x < result.getWidth(); ++x) {
		for (int y = 0; y < result.getHeight(); ++y) {
			result.setData(x, y, input.getData(x, y < (input.getHeight() / 2) ? y : input.getHeight() - 1 - y));
		}
	}
	return result;
}

std::shared_ptr<CFilter> CFilterMirrorHorizontal::clone() const {
	return make_shared<CFilterMirrorHorizontal>(*this);
}