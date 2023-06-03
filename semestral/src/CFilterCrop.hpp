#pragma once

#include "CFilter.hpp"

class CFilterCrop : public CFilter {
private:
	int m_fromX;
	int m_fromY;
	int m_width;
	int m_height;

public:
	CFilterCrop(int fromX, int fromY, int width, int height);
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual std::shared_ptr<CFilter> clone() const override;
};