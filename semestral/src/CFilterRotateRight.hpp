#pragma once

#include "CFilter.hpp"

/// @brief A filter, which rotates the image by 90 degrees to the right
class CFilterRotateRight : public CFilter {
public:
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual std::shared_ptr<CFilter> clone() const override;
};