#pragma once

#include <memory>

#include "CMatrix.hpp"

using namespace std;

/// @brief Parent class to filters
class CFilter {
public:
	/// @brief Applies a filter to a matrix
	/// @param input The matrix to use the filter on
	/// @return The resulting matrix
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const = 0;

	/// @brief Creates a shared_ptr of the filter
	/// @return shared_ptr to filter
	virtual std::shared_ptr<CFilter> clone() const = 0;
};