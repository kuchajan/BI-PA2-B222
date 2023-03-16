#ifndef __PROGTEST__
#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;
#endif /* __PROGTEST__ */

class CTimeStamp {
	private:
	uint64_t unixTimeStamp;
	bool isLeapYear(const int & year) {
		if ( year %  400 == 0) { return  true; }
		if ( year %  100 == 0) { return false; }
		if ( year %    4 == 0) { return  true; }
		return false;
	}

	int daysInMonth(int month, const int & year){
		if (month == 2) { return 28 + (isLeapYear(year) ? 1 : 0); }
		if (month > 7) { month -= 7; }
		if (month % 2 == 0) { return 30; }
		return 31;
	}

	int getYearDay(const int & day, const int & month, const int & year) {
		int yearDay = day - 1;
		for(int i = 1; i < month; i++) {
			yearDay += daysInMonth(month,year);
		}
		return yearDay;
	}
	public:
	CTimeStamp(const int & year, const int & month, const int & day, const int & hour, const int & minute, const int & second) {
		unixTimeStamp = second + minute*60 + hour*3600 + getYearDay(day,month,year)*86400 + (year-70)*31536000 + ((year-69)/4)*86400 - ((year-1)/100)*86400 + ((year+299)/400)*86400;
	}

	uint64_t getTimeStamp() {
		return unixTimeStamp;
	}

	bool operator <= (const CTimeStamp & other) const {
		return this->unixTimeStamp <= other.unixTimeStamp;
	}
};

class CContact {
	private:
	CTimeStamp stamp;
	int tel1;
	int tel2;

	public:
	CContact(const CTimeStamp & timeStamp, const int & telephone1, const int & telephone2)
	:stamp(timeStamp) {
		tel1 = telephone1;
		tel2 = telephone2;
	}
	inline bool contains(const int & tel) const {
		return (tel1 != tel2) && ((tel == tel1) || (tel == tel2));
	}
	int getOtherPhone(const int & tel) const {
		if (tel != tel1) {
			return tel1;
		}
		return tel2;
	}
};

class CEFaceMask {
	private:
	vector<CContact> contacts;

	public:
	CEFaceMask addContact(const CContact & contact) {
		contacts.push_back(contact);
		return *this;
	}

	vector<int> listContacts(const int & tel) const {
		vector<int> list;
		for (int i = 0; i < contacts.size() - 1; i++) {
			if(contacts[i].contains(tel)) {
				int toAdd = contacts[i].getOtherPhone(tel);
				bool isContained = false;
				for(int j = 0; j < list.size() - 1; j++) {
					if(list[j] == toAdd);
					isContained = true;
					break;
				}
				if(!isContained) {
					list.push_back(toAdd);
				}
			}
		}
	}
};

#ifndef __PROGTEST__
int main() {
	CEFaceMask test;

	test.addContact(CContact(CTimeStamp(2021, 1, 10, 12, 40, 10), 123456789, 999888777));
	test.addContact(CContact(CTimeStamp(2021, 1, 12, 12, 40, 10), 123456789, 111222333))
		.addContact(CContact(CTimeStamp(2021, 2, 5, 15, 30, 28), 999888777, 555000222));
	test.addContact(CContact(CTimeStamp(2021, 2, 21, 18, 0, 0), 123456789, 999888777));
	test.addContact(CContact(CTimeStamp(2021, 1, 5, 18, 0, 0), 123456789, 456456456));
	test.addContact(CContact(CTimeStamp(2021, 2, 1, 0, 0, 0), 123456789, 123456789));
	assert(test.listContacts(123456789) == (vector<int>{999888777, 111222333, 456456456}));
	assert(test.listContacts(999888777) == (vector<int>{123456789, 555000222}));
	assert(test.listContacts(191919191) == (vector<int>{}));
	assert(test.listContacts(123456789, CTimeStamp(2021, 1, 5, 18, 0, 0), CTimeStamp(2021, 2, 21, 18, 0, 0)) == (vector<int>{999888777, 111222333, 456456456}));
	assert(test.listContacts(123456789, CTimeStamp(2021, 1, 5, 18, 0, 1), CTimeStamp(2021, 2, 21, 17, 59, 59)) == (vector<int>{999888777, 111222333}));
	assert(test.listContacts(123456789, CTimeStamp(2021, 1, 10, 12, 41, 9), CTimeStamp(2021, 2, 21, 17, 59, 59)) == (vector<int>{111222333}));
	return 0;
}
#endif /* __PROGTEST__ */