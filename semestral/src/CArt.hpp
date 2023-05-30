#pragma once

#include <iostream>
#include <vector>

#include "CCharset.hpp"
#include "CFilter.hpp"
#include "CImage.hpp"
#include "CMatrix.hpp"

using namespace std;

class CArt {
private:
	CMatrix<char> m_result;

	CMatrix<unsigned char> getGrayScale(const CImage &image, const vector<shared_ptr<CFilter>> &filters) const;
	CMatrix<char> getResult(const CMatrix<unsigned char> &grayScale, const CCharset &charset) const;

public:
	CArt(const CImage &image, const CCharset &charset, const vector<shared_ptr<CFilter>> &filters);
	friend ostream &operator<<(ostream &os, const CArt &art);
};