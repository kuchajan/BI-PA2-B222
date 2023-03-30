#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX

class CRangeList;

class CRange {
private:
	friend CRangeList;
	// todo
	long long m_Low;
	long long m_High;

	inline bool isValid() const;
public:
	// constructor
	CRange(const long long &low, const long long &high);
	
	inline bool includes(const long long & value) const;
	inline bool includes(const CRange & range) const;
	inline bool overlays(const CRange & range) const;
	
	CRange & unite(const CRange & other);

	friend ostream & operator<<(ostream & os, const CRange & cr);
};

/// @brief: Constructs a CRange with a given lower and upper bound. Throws an exception if low > high
/// @param[in] low: Lower bound of the range 
/// @param[in] high: Upper bound of the range
/// @exception: low > high
CRange::CRange(const long long &low, const long long &high) {
	m_Low = low;
	m_High = high;
	if (!isValid()) {
		throw std::logic_error("CRange constructor: lower bound of range is bigger than upper bound");
	}
}

/// @brief: Checks if parent is a valid range
/// @return: true, when valid. false, when invalid
inline bool CRange::isValid() const {
	return m_Low <= m_High;
}

/// @brief Checks, if the given value is included in this range
/// @param value The value to check
/// @return true, if it's included, otherwise false
inline bool CRange::includes(const long long &value) const {
	return (m_Low <= value) && (value <= m_High);
}

/// @brief Checks, if the whole given range is included in this range
/// @param range The other range to check
/// @return true, if it incldues the whole range, otherwise false
inline bool CRange::includes(const CRange &range) const {
	return ((*this).m_Low <= range.m_Low) && (range.m_High <= (*this).m_High);
}

/// @brief Checks, if this range is partly overlaying the other range
/// @param range The other range to check
/// @return true, if it is overlaying, otherwise false
inline bool CRange::overlays(const CRange &range) const {
	return this->includes(range.m_High+1) || this->includes(range.m_Low-1);
}

CRange & CRange::unite(const CRange &other) {
	m_Low = m_Low <= other.m_Low ? m_Low : other.m_Low;
	m_High = m_High >= other.m_High ? m_High : other.m_High;
	return *this;
}

class CRangeList {
private:
	// todo
	vector<CRange> m_Ranges;
public:
	// constructor
	CRangeList();
	
	// includes long long / range
	// += range / range list
	CRangeList & operator+=(const CRange &);
	CRangeList & operator+=(const CRangeList &);
	// -= range / range list
	CRangeList & operator-=(const CRange &);
	CRangeList & operator-=(const CRangeList &);
	// = range / range list
	CRangeList & operator=(const CRange &);
	// CRangeList & operator=(const CRange &); //*Implicit
	// operator ==, !=
	bool operator==(const CRangeList &) const;
	bool operator!=(const CRangeList &) const;
	// includes long long / CRange
	bool includes(const long long &) const;
	bool includes(const CRange &) const;
	// operator <<
	friend ostream & operator << (ostream & os, const CRangeList & crl);
	// + range
	CRangeList & operator+(const CRange &);
};

/// @brief: Implicit empty constructor
CRangeList::CRangeList() { }

/// @brief: Destructor that empties the vector m_Ranges
/*CRangeList::~CRangeList() {
	m_Ranges.clear();
}*/

CRangeList &CRangeList::operator+=(const CRange &otherRange) {
	auto iterator = m_Ranges.begin();
	for(;iterator != m_Ranges.end(); iterator++) {
		if((*iterator).includes(otherRange)) {
			return *this;
		}
		if((*iterator).overlays(otherRange) || otherRange.overlays(*iterator)) {
			//union of the two ranges
			(*iterator).unite(otherRange);

			//union of all the subsequent ranges
			auto iterator2 = iterator+1;
			for(;(*iterator).overlays(*(iterator2));iterator2++) {
				(*iterator).unite(*(iterator2));
				if(iterator2 == m_Ranges.end()) {
					break;
				}
			}
			m_Ranges.erase(iterator+1,iterator2);
			return *this;
		}
		if((*iterator).m_Low > otherRange.m_Low) {
			break;
		}
	}
	m_Ranges.insert(iterator,otherRange);
	return *this;
}

CRangeList &CRangeList::operator+=(const CRangeList &otherList) {
	for(auto range : otherList.m_Ranges) {
		*this += range;
	}
	return *this;
}

CRangeList &CRangeList::operator-=(const CRange & otherRange) {
	for(auto iterator = m_Ranges.begin(); iterator != m_Ranges.end() && !(otherRange.m_High < (*iterator).m_Low); iterator++) {
		if(otherRange.m_Low > (*iterator).m_High) {
			//too low to do any op
			continue;
		}
		if(otherRange.includes(*iterator)) {
			//delete
			m_Ranges.erase(iterator);
			continue; //it could be equal, but let's still see
		}

		if((*iterator).includes(otherRange)) {
			//split to two
			long long lowTemp = (*iterator).m_Low;
			long long highTemp = (*iterator).m_High;
			//first the right hand side
			m_Ranges.insert(iterator,CRange(otherRange.m_High+1,highTemp));
			//then the left hand side
			m_Ranges.insert(iterator,CRange(lowTemp,otherRange.m_Low-1));
			//then remove the last one
			m_Ranges.erase(iterator+2);
			
			break; //i am sure it can't continue
		}

		//I have (5, 10) and I wanna subtract (1,7)
		//my result should be (8,10)
		if(otherRange.m_High < (*iterator).m_High) {
			(*iterator).m_Low = otherRange.m_High+1;
			continue;
		}

		//I have (5,10) and I wanna subtract (8,12)
		//my result should be (5,7)
		if(otherRange.m_Low < (*iterator).m_High) {
			(*iterator).m_High = otherRange.m_Low-1;
			continue;
		}

		//Maybe I forgot to implement some situation
		cerr << (*iterator) << " \\ " << otherRange << endl;
		throw logic_error("Unhandled situation, check cerr");
	}
	return *this;
}

CRangeList &CRangeList::operator-=(const CRangeList & otherList) {
	for(auto range : otherList.m_Ranges) {
		*this -= range;
	}
	return *this;
}

/// @brief Empties the current range list and adds given range
/// @param range The range to add to the list
/// @return this object
CRangeList &CRangeList::operator=(const CRange & range) {
	m_Ranges.clear();
	m_Ranges.push_back(range);
	return (*this);
}

bool CRangeList::operator==(const CRangeList & otherList) const {
	auto thisIter = this->m_Ranges.cbegin();
	auto otherIter = otherList.m_Ranges.cbegin();

	while(thisIter != this->m_Ranges.cend() && otherIter != otherList.m_Ranges.cend()) {
		if((*thisIter).m_Low != (*otherIter).m_Low || (*thisIter).m_High != (*otherIter).m_High) {
			return false;
		}
		thisIter++;
		otherIter++;
	}

	return !((thisIter == this->m_Ranges.cend() && otherIter != otherList.m_Ranges.cend())
	      || (otherIter == otherList.m_Ranges.cend() && thisIter != this->m_Ranges.cend()));
}

bool CRangeList::operator!=(const CRangeList & otherList) const {
	return !(*this == otherList);
}

bool CRangeList::includes(const long long & toFind) const {
	for(auto range : this->m_Ranges) {
		if(range.includes(toFind)) {
			return true;
		}
	}
	
	return false;
}

bool CRangeList::includes(const CRange & toFind) const {
	for(auto range : this->m_Ranges) {
		if(range.includes(toFind)) {
			return true;
		}
	}
	
	return false;
}

ostream &operator<<(ostream &os, const CRange &cr) {
	os << "<" << cr.m_Low << ".." << cr.m_High << ">";
	return os;
}

ostream &operator<<(ostream &os, const CRangeList &crl) {
	os << "{";
	bool isFirst = true;
	for(auto range : crl.m_Ranges) {
		if(!isFirst) {
			os << ",";
		}
		os << range;
		isFirst = false;
	}

	os << "}";
	return os;
}

CRangeList &CRangeList::operator+(const CRange & range) {
	*this += range;
	return *this;
}

CRangeList operator+(const CRange & left, const CRange & right) {
	CRangeList crl;
	crl += left;
	crl += right;

	return crl;
}

#ifndef __PROGTEST__
string toString(const CRangeList &x) {
	ostringstream oss;
	oss << x;
	return oss.str();
}

int main(void) {
	CRangeList a, b;

	assert(sizeof(CRange) <= 2 * sizeof(long long));

	//my tests
	a = CRange(5,10);
	a -= CRange(1,7);
	assert(toString(a) == "{<8..10>}");

	a = CRange(5,10);
	a -= CRange(8,20);
	assert(toString(a) == "{<5..7>}");

	a = CRange(5, 10);
	a += CRange(25, 100);
	assert(toString(a) == "{<5..10>,<25..100>}");
	a += CRange(-5, 0);
	a += CRange(8, 50);
	assert(toString(a) == "{<-5..0>,<5..100>}");
	a += CRange(101, 105) + CRange(120, 150) + CRange(160, 180) + CRange(190, 210);
	assert(toString(a) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}");
	a += CRange(106, 119) + CRange(152, 158);
	assert(toString(a) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}");
	a += CRange(-3, 170);
	a += CRange(-30, 1000);
	assert(toString(a) == "{<-30..1000>}");
	b = CRange(-500, -300) + CRange(2000, 3000) + CRange(700, 1001);
	a += b;
	assert(toString(a) == "{<-500..-300>,<-30..1001>,<2000..3000>}");
	a -= CRange(-400, -400);
	assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}");
	a -= CRange(10, 20) + CRange(900, 2500) + CRange(30, 40) + CRange(10000, 20000);
	assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
	try {
		a += CRange(15, 18) + CRange(10, 0) + CRange(35, 38);
		assert("Exception not thrown" == nullptr);
	} catch (const std::logic_error &e) {
	} catch (...) {
		assert("Invalid exception thrown" == nullptr);
	}
	assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
	b = a;
	assert(a == b);
	assert(!(a != b));
	b += CRange(2600, 2700);
	assert(toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
	assert(a == b);
	assert(!(a != b));
	b += CRange(15, 15);
	assert(toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}");
	assert(!(a == b));
	assert(a != b);
	assert(b.includes(15));
	assert(b.includes(2900));
	assert(b.includes(CRange(15, 15)));
	assert(b.includes(CRange(-350, -350)));
	assert(b.includes(CRange(100, 200)));
	assert(!b.includes(CRange(800, 900)));
	assert(!b.includes(CRange(-1000, -450)));
	assert(!b.includes(CRange(0, 500)));
	a += CRange(-10000, 10000) + CRange(10000000, 1000000000);
	assert(toString(a) == "{<-10000..10000>,<10000000..1000000000>}");
	b += a;
	assert(toString(b) == "{<-10000..10000>,<10000000..1000000000>}");
	b -= a;
	assert(toString(b) == "{}");
	b += CRange(0, 100) + CRange(200, 300) - CRange(150, 250) + CRange(160, 180) - CRange(170, 170);
	assert(toString(b) == "{<0..100>,<160..169>,<171..180>,<251..300>}");
	b -= CRange(10, 90) - CRange(20, 30) - CRange(40, 50) - CRange(60, 90) + CRange(70, 80);
	assert(toString(b) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}");
#ifdef EXTENDED_SYNTAX
	CRangeList x{{5, 20}, {150, 200}, {-9, 12}, {48, 93}};
	assert(toString(x) == "{<-9..20>,<48..93>,<150..200>}");
	ostringstream oss;
	oss << setfill('=') << hex << left;
	for (const auto &v : x + CRange(-100, -100))
		oss << v << endl;
	oss << setw(10) << 1024;
	assert(oss.str() == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======");
#endif /* EXTENDED_SYNTAX */
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */