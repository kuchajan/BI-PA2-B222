/**
 * @author Jan Kuchař <kuchaj19@fit.cvut.cz>
 * @date 09/05/2023
 */

#pragma once

#include <iostream>

using namespace std;

class CCharset {
private:
	string m_charset;

public:
	CCharset(const char *charset);
	char getChar(const uint8_t &value);
};