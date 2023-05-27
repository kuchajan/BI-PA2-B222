#pragma once

class CRatio {
private:
	int m_numerator;
	int m_denominator;

	int cmp(const CRatio &other);

	int gcd(int first, int second);
	int lcm(int first, int second);

public:
	CRatio(int numerator, int denominator);
	CRatio(const CRatio &src);

	CRatio operator+(CRatio &other);
	CRatio operator-(CRatio &other);

	bool operator<(CRatio &other);
	bool operator>(CRatio &other);
	bool operator==(CRatio &other);
	bool operator!=(CRatio &other);
	bool operator<=(CRatio &other);
	bool operator>=(CRatio &other);

	int nearestLowestRoundNumerator(CRatio &other);
	int nearestHighestRoundNumerator(CRatio &other);
};