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
	CMatrix() {
		m_data == nullptr;
	}
	CMatrix(const int &width, const int &height) : m_width(width), m_height(height) {
		if (m_width == 0 || m_height == 0) {
			throw invalid_argument("CMatrix::CMatrix: Width and/or height of the matrix should not be 0.");
		}
		m_data = new T[m_width * m_height];
	}
	CMatrix(const CMatrix &src) {
		delete[] m_data;

		m_width = src.m_width;
		m_height = src.m_height;

		m_data = new T[m_width * m_height];
		memcpy(m_data, src.m_data, m_width * m_height * sizeof(T));
	}
	~CMatrix() {
		delete[] m_data;
		m_data = nullptr;
	}
};