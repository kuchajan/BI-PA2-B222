#pragma once

#include "CFilter.hpp"

/// @brief A filter, which resizes the image to the given dimension
class CFilterResize : public CFilter {
private:
	/// @brief The new width of the image
	int m_newWidth;
	/// @brief The new height of the image
	int m_newHeight;

	/// @brief Calculates the average value of a rectangle inside a matrix
	/// @param input The input matrix
	/// @param fromX The collumn (inclusive) where the rectangle begins
	/// @param toX The collumn (exclusive) where the rectangle ends
	/// @param fromY The row (inclusive) where the rectangle begins
	/// @param toY The row (exclusive) where the rectangle ends
	/// @return The calculated average
	unsigned char average(const CMatrix<unsigned char> &input, int fromX, int toX, int fromY, int toY) const;

	/// @brief Calculates the range where to calculate the average from
	/// @param index The index (whether row or collumn) in the new matrix
	/// @param oldDimSize The old size of the dimension
	/// @param newDimSize The new size of the dimension
	/// @return A pair of ints with the beggining and end of a rectangle in a dimension
	pair<int, int> getRange(int index, int oldDimSize, int newDimSize) const;

public:
	/// @brief Creates a new resizing filter
	/// @param width The desired width of result
	/// @param height The desired height of result
	CFilterResize(int width, int height);
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const override;
	virtual shared_ptr<CFilter> clone() const override;
};