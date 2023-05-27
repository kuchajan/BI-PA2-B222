#include "CRatio.hpp"

using namespace std;

CRatio::CRatio(int numerator, int denominator) : m_numerator(numerator), m_denominator(denominator) {}

CRatio::CRatio(const CRatio &src) : m_numerator(src.m_numerator), m_denominator(src.m_denominator) {}
