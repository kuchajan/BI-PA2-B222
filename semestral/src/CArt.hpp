#pragma once

#include <iostream>

#include "CCharset.hpp"
#include "CImage.hpp"
#include "CMatrix.hpp"

using namespace std;

class CArt {
private:
	CMatrix<char> m_result;

	// todo: get average of numbers without overflow
	// todo: somehow parse the image in such a way that it can be smaller or larger than original

public:
	CArt(const CImage &image, const CCharset &charset, const int &width, const int &height);
	friend ostream &operator<<(ostream &os, const CArt &art);
};