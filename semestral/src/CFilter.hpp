#pragma once

#include <iostream>

#include "CMatrix.hpp"

class CFilter {
private:
protected:
public:
	virtual CMatrix<char> apply(CMatrix<char> input) = 0;
};