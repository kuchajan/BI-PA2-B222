#pragma once

#include "CFilter.hpp"

class CFilterContrast : public CFilter {
private:
	double m_value;

public:
	CFilterContrast(int value);
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual std::shared_ptr<CFilter> clone() const override;
};