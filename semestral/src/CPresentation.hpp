#pragma once

#include <ncurses.h>

#include <sstream>

#include "COutput.hpp"

/// @brief An outputter, which outputs the generated art in a form of an interactable presentation
class CPresentation : public COutput {
private:
	/// @brief uses ncurses to clear the screen and outputs art with the specified index
	/// @param idx The index of the image
	void showArt(size_t idx);

public:
	virtual void output() override;
	virtual shared_ptr<COutput> clone() override;
};