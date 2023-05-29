#pragma once

#include "CFilter.hpp"

class CFilterResize : public CFilter {
private:
	int m_newWidth;
	int m_newHeight;

	char average(const CMatrix<char> &input, int fromX, int toX, int fromY, int toY) const;
	pair<int, int> getRange(int index, int oldDimSize, int newDimSize) const;

public:
	CFilterResize(int width, int height);
	virtual CMatrix<char> apply(const CMatrix<char> &input) const override;
	virtual shared_ptr<CFilter> clone() const override;
};