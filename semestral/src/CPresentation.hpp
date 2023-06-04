#pragma once

#include <ncurses.h>

#include <sstream>

#include "COutput.hpp"

class CPresentation : public COutput {
private:
	void showArt(size_t idx);

public:
	virtual void
	output() override;
	virtual shared_ptr<COutput> clone() override;
};