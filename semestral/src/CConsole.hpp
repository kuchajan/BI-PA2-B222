#pragma once

#include "COutput.hpp"

/// @brief An outputter, which outputs the generated art in the console
class CConsole : public COutput {
private:
public:
	virtual void output() override;
	virtual shared_ptr<COutput> clone();
};