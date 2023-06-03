#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "CArt.hpp"

class COutput {
protected:
	vector<CArt> m_art;

public:
	virtual void output() = 0;
	virtual shared_ptr<COutput> clone() = 0;
	void addArts(vector<CArt> &src);
};