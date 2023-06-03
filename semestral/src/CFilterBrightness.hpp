#pragma once

#include "CFilter.hpp"

class CFilterBrightness : public CFilter {
private:
	int m_value;

public:
	CFilterBrightness(int value);
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual std::shared_ptr<CFilter> clone() const override;
};