#pragma once

#include "CFilter.hpp"

/// @brief A filter, which mirrors the image by the horizontal central axis
class CFilterMirrorHorizontal : public CFilter {
public:
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual std::shared_ptr<CFilter> clone() const override;
};