#include "CRatio.hpp"

using namespace std;

/// @brief Finds the greatest common divisor between two numbers
/// @return The greatest common divisor
int gcd(int first, int second) {
	while (first != second) {
		if (first > second) {
			first -= second;
		} else {
			second -= first;
		}
	}
	return first;
}

/// @brief Finds the lowest common multiple between two numbers
/// @return The lowest common multiple
int lcm(int first, int second) {
	return first * second / gcd(first, second);
}

CRatio::CRatio(int numerator, int denominator) : m_numerator(numerator), m_denominator(denominator) {}

CRatio::CRatio(const CRatio &src) : m_numerator(src.m_numerator), m_denominator(src.m_denominator) {}
