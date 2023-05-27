#pragma once

int gcd(int first, int second);
int lcm(int first, int second);

class CRatio {
private:
	int m_numerator;
	int m_denominator;

	int cmp(const CRatio &other) const;

public:
	CRatio(int numerator, int denominator);
	CRatio(const CRatio &src);

	CRatio operator+(CRatio &other);
	CRatio operator-(CRatio &other);

	bool operator<(CRatio &other) const;
	bool operator>(CRatio &other) const;
	bool operator==(CRatio &other) const;
	bool operator!=(CRatio &other) const;
	bool operator<=(CRatio &other) const;
	bool operator>=(CRatio &other) const;

	int nearestLowestRoundNumerator(CRatio &other) const;
	int nearestHighestRoundNumerator(CRatio &other) const;
};