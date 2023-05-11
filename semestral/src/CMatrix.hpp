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

	inline bool checkPosValid(const int &collumn, const int &row) const {
		return collumn < m_width && row < m_height;
	}
	inline bool checkIndexValid(const size_t &index) const {
		return index < getElementCount();
	}

	inline size_t getIndex(const int &collumn, const int &row) const {
		return row * m_width + collumn;
	}

public:
	CMatrix() {
		m_data = nullptr;
		m_width = 0;
		m_height = 0;
	}
	CMatrix(const int &width, const int &height) : m_width(width), m_height(height) {
		if (m_width == 0 || m_height == 0) {
			throw invalid_argument("CMatrix::CMatrix: Width and/or height of the matrix should not be 0.");
		}
		m_data = new T[m_width * m_height];
	}
	CMatrix(const CMatrix &src) {
		m_width = src.m_width;
		m_height = src.m_height;

		m_data = new T[m_width * m_height];
		memcpy(m_data, src.m_data, m_width * m_height * sizeof(T));
	}
	~CMatrix() {
		delete[] m_data;
		m_data = nullptr;
	}

	CMatrix operator=(CMatrix src) {
		swap(m_data, src.m_data);
		swap(m_width, src.m_width);
		swap(m_height, src.m_height);

		return *this;
	}

	int getHeight() const {
		return m_height;
	}
	int getWidth() const {
		return m_width;
	}
	size_t getElementCount() const {
		return m_height * m_width;
	}

	T getData(const int &collumn, const int &row) const {
		if (!checkPosValid(collumn, row)) {
			throw invalid_argument("CMatrix::getData: collumn or row is invalid.");
		}
		return m_data[getIndex(collumn, row)];
	}
	T getData(const size_t &index) const {
		if (!checkIndexValid(index)) {
			throw invalid_argument("CMatrix::getData: index is invalid.");
		}
		return m_data[index];
	}

	void setData(const int &collumn, const int &row, const T &data) {
		if (!checkPosValid(collumn, row)) {
			throw invalid_argument("CMatrix::setData: collumn or row is invalid.");
		}
		m_data[getIndex(collumn, row)] = data;
	}
	void setData(const size_t &index, const T &data) {
		if (!checkIndexValid(index)) {
			throw invalid_argument("CMatrix::setData: index is invalid.");
		}
		m_data[index] = data;
	}
};