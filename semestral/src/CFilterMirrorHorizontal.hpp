#pragma once

#include "CFilter.hpp"

class CFilterMirrorHorizontal : public CFilter {
public:
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual std::shared_ptr<CFilter> clone() const override;
};