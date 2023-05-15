#pragma once

#include <vector>

#include "CArt.hpp"

class COutput {
private:
protected:
	vector<CArt> art;

public:
	virtual void output() = 0;
};