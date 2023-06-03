#pragma once

#include "COutput.hpp"

class CConsole : public COutput {
private:
public:
	virtual void output() override;
	virtual shared_ptr<COutput> clone();
};