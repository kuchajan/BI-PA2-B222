#pragma once

#include "CMatrix.hpp"

class CFilter {
private:
protected:
public:
	virtual CMatrix<char> apply(const CMatrix<char> &input) const = 0;
};