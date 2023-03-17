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
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	
	public:
	CTimeStamp(const int & y, const int & mon, const int & d, const int & h, const int & min, const int & s) {
		year = y;
		month = mon;
		day = d;
		hour = h;
		minute = min;
		second = s;
	}

	bool operator <= (const CTimeStamp & other) const {
		if (this->year != other.year) {
			return (this->year < other.year);
		}
		if (this->month != other.month) {
			return (this->month < other.month);
		}
		if (this->day != other.day) {
			return (this->day < other.day);
		}
		if (this->hour != other.hour) {
			return (this->hour < other.hour);
		}
		if (this->minute != other.minute) {
			return (this->minute < other.minute);
		}
		return (this->second <= other.second); 
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

	bool isInInterval(const CTimeStamp & begin, const CTimeStamp & end) const {
		return ((begin <= stamp) && (stamp <= end));
	}
};

class CEFaceMask {
	private:
	vector<CContact> contacts;

	void addToListIfNotContained(const int & toAdd, vector<int> & list) const {
		for(size_t j = 0; (j < list.size()) && !list.empty(); j++) {
			if(list[j] == toAdd) {
				return;
			}
		}
		list.push_back(toAdd);
	}

	public:
	CEFaceMask & addContact(const CContact & contact) {
		contacts.push_back(contact);
		return *this;
	}

	vector<int> listContacts(const int & tel) const {
		vector<int> list;
		for (size_t i = 0; (i < contacts.size()) && !contacts.empty(); i++) {
			if(contacts[i].contains(tel)) {
				addToListIfNotContained(contacts[i].getOtherPhone(tel),list);
			}
		}
		return list;
	}

	vector<int> listContacts(const int & tel, const CTimeStamp & begin, const CTimeStamp & end) const {
		vector<int> list;
		for (size_t i = 0; i < (contacts.size()) && !contacts.empty(); i++) {
			if(contacts[i].contains(tel) && contacts[i].isInInterval(begin,end)) {
				addToListIfNotContained(contacts[i].getOtherPhone(tel),list);
			}
		}
		return list;
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