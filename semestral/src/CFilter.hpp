#pragma once

#include <memory>

#include "CMatrix.hpp"

using namespace std;

class CFilter {
public:
	virtual CMatrix<unsigned char> apply(const CMatrix<unsigned char> &input) const = 0;
	virtual std::shared_ptr<CFilter> clone() const = 0;
};