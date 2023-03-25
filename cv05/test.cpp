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

    // comparison operators

    // output operator

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