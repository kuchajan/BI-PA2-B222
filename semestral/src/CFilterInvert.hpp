#pragma once

#include "CFilter.hpp"

/// @brief A filter, which inverts the gray scale
class CFilterInvert : public CFilter {
public:
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual std::shared_ptr<CFilter> clone() const override;
};