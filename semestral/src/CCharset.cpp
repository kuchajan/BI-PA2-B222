/**
 * @author Jan Kuchař <kuchaj19@fit.cvut.cz>
 * @date 09/05/2023
 */
#include <iostream>

#include "CCharset.hpp"

using namespace std;

/// @brief Creates a new charset to use to generate art
/// @param charset String of characters to use
/// @exception Throws an invalid_argument when charset length is 0 or more than 256
CCharset::CCharset(const char *charset) : m_charset(charset) {
	size_t length = m_charset.length();
	if (length == 0 || length > 256) {
		throw invalid_argument("CCharset::CCharset: Charset cannot be empty or with more than 256 characters.");
	}
}

char CCharset::getChar(const uint8_t &value) const {
	return m_charset[(value * (m_charset.length())) / 256];
}
