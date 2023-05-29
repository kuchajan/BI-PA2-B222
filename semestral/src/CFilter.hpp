#pragma once

#include <memory>

#include "CMatrix.hpp"

using namespace std;

class CFilter {
private:
protected:
public:
	virtual CMatrix<char> apply(const CMatrix<char> &input) const = 0;
	virtual std::shared_ptr<CFilter> clone() const = 0;
};