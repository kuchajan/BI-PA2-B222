#include "CFilterMirrorVertical.hpp"

CMatrix<unsigned char> CFilterMirrorVertical::apply(const CMatrix<unsigned char> &input) const {
	CMatrix<unsigned char> result(input.getWidth(), input.getHeight());
	for (int x = 0; x < result.getWidth(); ++x) {
		for (int y = 0; y < result.getHeight(); ++y) {
			result.setData(x, y, input.getData(x < (input.getWidth() / 2) ? x : input.getWidth() - 1 - x, y));
		}
	}
	return result;
}

std::shared_ptr<CFilter> CFilterMirrorVertical::clone() const {
	return make_shared<CFilterMirrorVertical>(*this);
}