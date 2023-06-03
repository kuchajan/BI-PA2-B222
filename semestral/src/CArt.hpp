#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "CCharset.hpp"
#include "CFilter.hpp"
#include "CImage.hpp"
#include "CMatrix.hpp"

using namespace std;

class CArt {
private:
	char *m_path;
	CCharset m_charset;
	vector<shared_ptr<CFilter>> m_filters;

	CMatrix<unsigned char> getGrayScale() const;
	CMatrix<char> getResult() const;

public:
	CArt(char *path);
	void setCharset(CCharset cs);
	void addFilter(shared_ptr<CFilter> filter);
	friend ostream &operator<<(ostream &os, const CArt &art);
};