#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "CArt.hpp"

/// @brief Specifies a way to output the generated art
class COutput {
protected:
	/// @brief An array of all generated art
	vector<CArt> m_art;

public:
	/// @brief A function to start the output of the images
	virtual void output() = 0;

	/// @brief Creates a shared_ptr to the outputter
	/// @return shared_ptr to this outputter
	virtual shared_ptr<COutput> clone() = 0;

	/// @brief Replaces the vector stored in outputter with the source vector
	/// @param src The source vector
	void addArts(vector<CArt> &src);
};