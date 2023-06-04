#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "CCharset.hpp"
#include "CFilter.hpp"
#include "CImage.hpp"
#include "CMatrix.hpp"

using namespace std;

/// @brief Generates art from a given path to an image
/// @note When no charset was set, the default " .:*I$VFNM" is used
class CArt {
private:
	///@brief Path to the image
	char *m_path;
	///@brief charset to use
	CCharset m_charset;
	///@brief Filters to apply
	vector<shared_ptr<CFilter>> m_filters;

	/// @brief Uses CImage to get a representation of a grayscale image
	/// @return An 8bit representation of a grayscale image in a matrix
	CMatrix<unsigned char> getGrayScale() const;

	/// @brief Applies a charset and filters to a grayscale matrix
	/// @return The resulting matrix
	CMatrix<char> getResult() const;

public:
	/// @brief Sets up path to an image to generate art from
	/// @param path The path to the image
	CArt(char *path);

	/// @brief Sets the charset to use when generating
	/// @param cs The charset to use
	void setCharset(CCharset cs);

	/// @brief Adds a filter to use when generating art
	/// @param filter The filter to add
	void addFilter(shared_ptr<CFilter> filter);

	/// @brief Generates the art and prints it to the stream
	/// @param os Stream to use
	/// @param art The art to generate
	/// @return Stream that was used
	friend ostream &operator<<(ostream &os, const CArt &art);
};