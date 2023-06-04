#pragma once

#include "CFilter.hpp"

/// @brief A filter, which changes the contrast of the image
class CFilterContrast : public CFilter {
private:
	/// @brief The contrast factor
	double m_value;

public:
	/// @brief Creates a contrast filter
	/// @param value How to change contrast
	/// @throw invalid_argument if the value is not in the range <-255, 255>
	CFilterContrast(int value);
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual std::shared_ptr<CFilter> clone() const override;
};