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

/// @brief Acts like a spaceship operator for two ratios
/// @return Whether this is lower (-1), equal (0), or higher (1) than the other ratio
int CRatio::cmp(const CRatio &other) const {
	int commonDenom = lcm(m_denominator, other.m_denominator);
	int thisExpanded = (commonDenom / m_denominator) * m_numerator;
	int otherExpanded = (commonDenom / other.m_denominator) * other.m_numerator;

	return (thisExpanded < otherExpanded ? -1 : (thisExpanded == otherExpanded ? 0 : 1));
}

CRatio::CRatio(int numerator, int denominator) : m_numerator(numerator), m_denominator(denominator) {}

CRatio::CRatio(const CRatio &src) : m_numerator(src.m_numerator), m_denominator(src.m_denominator) {}

bool CRatio::operator<(CRatio &other) const {
	return cmp(other) == -1;
}
bool CRatio::operator>(CRatio &other) const {
	return cmp(other) == 1;
}
bool CRatio::operator==(CRatio &other) const {
	return cmp(other) == 0;
}
bool CRatio::operator!=(CRatio &other) const {
	return cmp(other) != 0;
}
bool CRatio::operator<=(CRatio &other) const {
	return cmp(other) != 1;
}
bool CRatio::operator>=(CRatio &other) const {
	return cmp(other) != -1;
}

int CRatio::nearestLowestRoundNumerator(CRatio &other) const {
	return (other.m_numerator * this->m_denominator) / m_denominator;
}

int CRatio::nearestHighestRoundNumerator(CRatio &other) const {
	// example: for this 1/40 = and other = 5/30, return (7)/40
	int tmp = (other.m_numerator * this->m_denominator);
	int res = tmp / m_denominator;
	// (res * m_denominator == tmp) is another way to say (tmp % m_denominator == 0), just more efficient (I think)
	return (res * m_denominator == tmp) ? res : res + 1;
}
