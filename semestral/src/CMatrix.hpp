/**
 * @author Jan Kuchař <kuchaj19@fit.cvut.cz>
 * @date 08/05/2023
 */

#pragma once

#include <iostream>
#include <stdexcept>

using namespace std;

/// @brief A matrix containing data of any type
/// @tparam T The type of data to be contained
template <typename T>
class CMatrix {
private:
	int m_width;
	int m_height;
	T *m_data;

public:
};