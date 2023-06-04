#pragma once

#include "CFilter.hpp"

/// @brief A filter, which flips the image by the vertical central axis
class CFilterFlipVertical : public CFilter {
public:
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual std::shared_ptr<CFilter> clone() const override;
};