#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class CTimeTester;
#endif /* __PROGTEST__ */

class CTime
{
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

	inline bool argsValid(int hour, int minute, int second) {
		return hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60;
	}

	void normalize() {
		m_Hour = (m_Hour + ((m_Minute + (m_Second/60)) / 60)) % 24;
		m_Minute = (m_Minute + (m_Second/60)) % 60;
		m_Second = m_Second % 60;
	}

	int toSeconds() const {
		return (((m_Hour * 60) + m_Minute) * 60) + m_Second;
	}
public:
    // constructor, destructor
	CTime() {
		m_Hour = 0;
		m_Minute = 0;
		m_Second = 0;
	}
	CTime(int hour, int minute) {
		if(!argsValid(hour,minute,0)) {
			throw std::invalid_argument("CTime(int,int): invalid arguments");
		}
		m_Hour = hour;
		m_Minute = minute;
		m_Second = 0;
	}
	CTime(int hour, int minute, int second) {
		if(!argsValid(hour,minute,second)) {
			throw std::invalid_argument("CTime(int,int,int): invalid arguments");
		}
		m_Hour = hour;
		m_Minute = minute;
		m_Second = second;
	}
    // arithmetic operators
	CTime & operator+=(int toAdd) {
		m_Second += toAdd;
		normalize();
		return *this;
	}
	CTime operator+(int toAdd) const {
		CTime time(*this);
		time += toAdd;
		return time;
	}

	CTime & operator-=(int toSub) {
		m_Second -= toSub;
		normalize();
		return *this;
	}
	CTime operator-(int toSub) const {
		CTime time(*this);
		time -= toSub;
		return time;
	}

	int operator-(CTime & other) const {
		int seconds = abs((*this).toSeconds() - other.toSeconds());
		seconds = seconds > 12*60*60 ? 24*60*60 - seconds : seconds;
		return seconds;
	}

	CTime operator++(int) {
		CTime time(*this);
		*this += 1;
		return time;
	}

	CTime & operator++() {
		*this += 1;
		return *this;
	}

	CTime operator--(int) {
		CTime time(*this);
		*this -= 1;
		return time;
	}

	CTime & operator--() {
		*this -= 1;
		return *this;
	}

    // comparison operators
	bool operator<(const CTime & other) const {
		return toSeconds() < other.toSeconds();
	}
	bool operator>(const CTime & other) const {
		return toSeconds() > other.toSeconds();
	}
	bool operator==(const CTime & other) const {
		return toSeconds() == other.toSeconds();
	}
	bool operator<=(const CTime & other) const {
		return (*this).operator==(other) || (*this).operator<(other);
	}
	bool operator>=(const CTime & other) const {
		return (*this).operator==(other) || (*this).operator>(other);
	}
	bool operator!=(const CTime & other) const {
		return !((*this).operator==(other));
	}
	
    // output operator
	friend ostream & operator<<(ostream & os, const CTime & time) {
		os << setw(2) << setfill('0') << time.m_Hour << ":" << setw(2) << time.m_Minute << ":" << setw(2) << time.m_Second;
		return os;
	}

    friend class ::CTimeTester;
};

#ifndef __PROGTEST__
struct CTimeTester
{
    static bool test ( const CTime & time, int hour, int minute, int second )
    {
        return time.m_Hour == hour 
            && time.m_Minute == minute
            && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main ()
{
    CTime a { 12, 30 };
    assert( CTimeTester::test( a, 12, 30, 0 ) );

    CTime b { 13, 30 };
    assert( CTimeTester::test( b, 13, 30, 0 ) );

    assert( b - a == 3600 );

    assert( CTimeTester::test( a + 60, 12, 31, 0 ) );
    assert( CTimeTester::test( a - 60, 12, 29, 0 ) );

    assert( a < b );
    assert( a <= b );
    assert( a != b );
    assert( !( a > b ) );
    assert( !( a >= b ) );
    assert( !( a == b ) );

    while ( ++a != b );
    assert( a == b );

    std::ostringstream output;
    assert( static_cast<bool>( output << a ) );
    assert( output.str() == "13:30:00" );

    assert( a++ == b++ );
    assert( a == b );

    assert( --a == --b );
    assert( a == b );

    assert( a-- == b-- );
    assert( a == b );

    return 0;
}
#endif /* __PROGTEST__ */