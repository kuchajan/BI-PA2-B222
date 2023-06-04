#pragma once

#include "CFilter.hpp"

/// @brief A filter, which crops the image from the given position with the given size
class CFilterCrop : public CFilter {
private:
	/// @brief The collumn from which to start cropping the image
	int m_fromX;
	/// @brief The row from which to start cropping the image
	int m_fromY;
	/// @brief The width of the desired output
	int m_width;
	/// @brief The height of the desired output
	int m_height;

public:
	/// @brief Creates a new cropping filter
	/// @param fromX The collumn from which to start cropping the image
	/// @param fromY The row from which to start cropping the image
	/// @param width The width of the desired output
	/// @param height The height of the desired output
	CFilterCrop(int fromX, int fromY, int width, int height);
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual std::shared_ptr<CFilter> clone() const override;
};