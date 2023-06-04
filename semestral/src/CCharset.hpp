/**
 * @author Jan Kuchař <kuchaj19@fit.cvut.cz>
 * @date 09/05/2023
 */

#pragma once

#include <iostream>

using namespace std;

/// @brief A character set which will be used to generate art from
class CCharset {
private:
	/// @brief The charset
	string m_charset;

public:
	/// @brief Creates a new charset to use to generate art
	/// @param charset String of characters to use
	/// @throws invalid_argument when charset length is 0 or more than 256
	CCharset(const char *charset);

	/// @brief Gets a character from the charset by the given value
	/// @param value The value to determine which character to return
	/// @return The desired character
	char getChar(uint8_t value) const;
};