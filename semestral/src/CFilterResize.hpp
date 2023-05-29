#pragma once

#include "CFilter.hpp"

class CFilterResize : public CFilter {
private:
	int m_newWidth;
	int m_newHeight;

	unsigned char average(const CMatrix<unsigned char> &input, int fromX, int toX, int fromY, int toY) const;
	pair<int, int> getRange(int index, int oldDimSize, int newDimSize) const;

public:
	CFilterResize(int width, int height);
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual shared_ptr<CFilter> clone() const override;
};