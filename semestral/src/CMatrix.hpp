/**
 * @author Jan Kuchař <kuchaj19@fit.cvut.cz>
 * @date 08/05/2023
 */

#pragma once

#include <cstring>
#include <iostream>
#include <stdexcept>

using namespace std;

/// @brief A matrix containing data of any type
/// @tparam T The type of data to be contained
template <typename T>
class CMatrix {
private:
	int m_width;  /// The width of the matrix
	int m_height; /// The height of the matrix
	T *m_data;	  /// Pointer to dynamically allocated data

	/// @brief Checks, whether a given collumn and row are valid numbers
	/// @param collumn The collumn to check
	/// @param row The row to check
	/// @return True when valid, otherwise false
	inline bool checkPosValid(int collumn, int row) const {
		return collumn < m_width && row < m_height;
	}

	/// @brief Checks, whether a given index is valid
	/// @param index The index to check
	/// @return True when valid, otherwise false
	inline bool checkIndexValid(const size_t &index) const {
		return index < getElementCount();
	}

	/// @brief Gets the index from a collumn and a row
	/// @param collumn The collumn
	/// @param row The row
	/// @return The index, calculated from the collumn and row
	inline size_t getIndex(const int &collumn, const int &row) const {
		return row * m_width + collumn;
	}

public:
	/// @brief Constructs an empty matrix with no data and no size
	CMatrix() {
		m_data = nullptr;
		m_width = 0;
		m_height = 0;
	}

	/// @brief Constructs a matrix with the given width and height
	/// @param width The width of the matrix
	/// @param height The height of the matrix
	CMatrix(int width, int height) : m_width(width), m_height(height) {
		if (m_width == 0 || m_height == 0) {
			throw invalid_argument("CMatrix::CMatrix: Width and/or height of the matrix should not be 0.");
		}
		m_data = new T[m_width * m_height];
	}

	/// @brief A copy constructor, which creates a copy from a matrix
	/// @param src The source matrix to copy from
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

	/// @brief Creates a copy of the right hand side and swaps data
	/// @param src The source matrix to copy
	/// @return Swapped copy of right hand side
	CMatrix operator=(CMatrix src) {
		swap(m_data, src.m_data);
		swap(m_width, src.m_width);
		swap(m_height, src.m_height);

		return *this;
	}

	/// @brief Getter of the height of the matrix
	/// @return The height
	int getHeight() const {
		return m_height;
	}

	/// @brief Getter of the width of the matrix
	/// @return The width
	int getWidth() const {
		return m_width;
	}

	/// @brief Calculates the total element count of the matrix
	/// @return The total element count
	size_t getElementCount() const {
		return m_height * m_width;
	}

	/// @brief Gets data from the matrix from the given collumn and row
	/// @param collumn The collumn
	/// @param row The row
	/// @return The data from the given position
	/// @throws invalid_argument when collumn or row is invalid
	T getData(int collumn, int row) const {
		if (!checkPosValid(collumn, row)) {
			throw invalid_argument("CMatrix::getData: collumn or row is invalid.");
		}
		return m_data[getIndex(collumn, row)];
	}

	/// @brief Gets data from the matrix from the given index
	/// @param index The index
	/// @return The data from the given index
	/// @throws invalid_argument when index is invalid
	T getData(size_t index) const {
		if (!checkIndexValid(index)) {
			throw invalid_argument("CMatrix::getData: index is invalid.");
		}
		return m_data[index];
	}

	/// @brief Sets data in the matrix in the given collumn and row
	/// @param collumn The collumn
	/// @param row The row
	/// @param data The value to set
	/// @throws invalid_argument when collumn or row is invalid
	void setData(int collumn, int row, const T &data) {
		if (!checkPosValid(collumn, row)) {
			throw invalid_argument("CMatrix::setData: collumn or row is invalid.");
		}
		m_data[getIndex(collumn, row)] = data;
	}

	/// @brief Sets data in the matrix in the given index
	/// @param index The index
	/// @param data The value to set
	/// @throws invalid_argument when index is invalid
	void setData(size_t index, const T &data) {
		if (!checkIndexValid(index)) {
			throw invalid_argument("CMatrix::setData: index is invalid.");
		}
		m_data[index] = data;
	}
};