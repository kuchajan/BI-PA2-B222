#include "CFilterCrop.hpp"

CFilterCrop::CFilterCrop(int fromX, int fromY, int width, int height) : m_fromX(fromX), m_fromY(fromY), m_width(width), m_height(height) {}

CMatrix<unsigned char> CFilterCrop::apply(const CMatrix<unsigned char> &input) const {
	CMatrix<unsigned char> result(m_width, m_height);
	for (int x = 0; x < m_width; ++x) {
		for (int y = 0; y < m_height; ++y) {
			result.setData(x, y, input.getData(x + m_fromX, y + m_fromY));
		}
	}
	return result;
}

std::shared_ptr<CFilter> CFilterCrop::clone() const {
	return make_shared<CFilterCrop>(*this);
}
