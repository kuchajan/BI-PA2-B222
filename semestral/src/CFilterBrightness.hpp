#pragma once

#include "CFilter.hpp"

/// @brief A filter, which changes the brightness of the image
class CFilterBrightness : public CFilter {
private:
	/// @brief How much to change the brightness by
	int m_value;

public:
	/// @brief Creates a new brightness filter
	/// @param value How to change the brightness
	/// @throws invalid_argument if the value is outside the range <-255, 255>
	CFilterBrightness(int value);
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual std::shared_ptr<CFilter> clone() const override;
};