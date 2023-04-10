#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

class CDate {
private:
	int16_t m_Year;
	int8_t m_Month;
	int8_t m_Day;

public:
	//---------------------------------------------------------------------------------------------
	CDate(int y, int m, int d)
		: m_Year(y), m_Month(m), m_Day(d) {
	}
	//---------------------------------------------------------------------------------------------
	int compare(const CDate &x) const {
		if (m_Year != x.m_Year)
			return m_Year - x.m_Year;
		if (m_Month != x.m_Month)
			return m_Month - x.m_Month;
		return m_Day - x.m_Day;
	}
	//---------------------------------------------------------------------------------------------
	int year(void) const {
		return m_Year;
	}
	//---------------------------------------------------------------------------------------------
	int month(void) const {
		return m_Month;
	}
	//---------------------------------------------------------------------------------------------
	int day(void) const {
		return m_Day;
	}
	//---------------------------------------------------------------------------------------------
	friend ostream &operator<<(ostream &os, const CDate &x) {
		char oldFill = os.fill();
		return os << setfill('0') << setw(4) << x.m_Year << "-"
				  << setw(2) << static_cast<int>(x.m_Month) << "-"
				  << setw(2) << static_cast<int>(x.m_Day)
				  << setfill(oldFill);
	}
};
#endif /* __PROGTEST__ */

class CCompany {
private:
	string m_originalName;
	string m_canonicalName;
	/// @brief converts a string to a canonical string, which contains only lowercase characters and no redundant spaces
	/// @param src string to convert
	/// @return a canonical string
	string toCanonical(const string &src) {
		// code shown on proseminar
		istringstream iss(src);
		ostringstream oss;
		copy(istream_iterator<string>(iss), istream_iterator<string>(), ostream_iterator<string>(oss, " ")); // get rid of extra white spaces
		string toReturn = oss.str();
		std::transform(toReturn.begin(), toReturn.end(), toReturn.begin(), [](unsigned char c) { return std::tolower(c); }); // transform all chars in string to lowercase
		return toReturn;
	}

	/// @brief Compares two company names by their canonical name
	/// @param other The other company to compare the canonical name with
	/// @return 0 if equal, less than 0 if first different char is lesser, more than 0 if first char is greater
	int compare(const CCompany &other) const {
		return m_canonicalName.compare(other.m_canonicalName);
	}

public:
	/// @brief Construct a new CCompany object given by the name
	/// @param name Name of the company
	CCompany(const string &name) : m_originalName(name), m_canonicalName(toCanonical(name)) {}

	/// @brief Uses CCompany::compare to compare two companies
	/// @param other The other company to compare with
	/// @return true if first different char is lesser, otherwise false
	bool operator<(const CCompany &other) const {
		return compare(other) < 0;
	}

	/// @brief Uses CCompany::compare to compare two companies
	/// @param other The other company to compare with
	/// @return true if first different char is greater, otherwise false
	bool operator>(const CCompany &other) const {
		return compare(other) > 0;
	}

	/// @brief Uses CCompany::compare to compare two companies
	/// @param other The other company to compare with
	/// @return true if equal or first different char is lesser, otherwise false
	bool operator<=(const CCompany &other) const {
		return compare(other) <= 0;
	}

	/// @brief Uses CCompany::compare to compare two companies
	/// @param other The other company to compare with
	/// @return true if equal or first different char is greater, otherwise false
	bool operator>=(const CCompany &other) const {
		return compare(other) >= 0;
	}

	/// @brief Uses CCompany::compare to compare two companies
	/// @param other The other company to compare with
	/// @return true if equal, otherwise false
	bool operator==(const CCompany &other) const {
		return compare(other) == 0;
	}

	/// @brief Uses CCompany::compare to compare two companies
	/// @param other The other company to compare with
	/// @return true if not equal, otherwise false
	bool operator!=(const CCompany &other) const {
		return compare(other) != 0;
	}
};

class CInvoice {
private:
	// todo

public:
	CInvoice(const CDate &date, const string &seller, const string &buyer,
			 unsigned int amount, double vat);
	CDate date(void) const;
	string buyer(void) const;
	string seller(void) const;
	unsigned int amount(void) const;
	double vat(void) const;
};

class CSortOpt {
private:
	// todo

public:
	static const int BY_DATE = 0;
	static const int BY_BUYER = 1;
	static const int BY_SELLER = 2;
	static const int BY_AMOUNT = 3;
	static const int BY_VAT = 4;
	CSortOpt(void);
	CSortOpt &addKey(int key, bool ascending = true);
};

class CVATRegister {
private:
	// todo

public:
	CVATRegister(void);
	bool registerCompany(const string &name);
	bool addIssued(const CInvoice &x);
	bool addAccepted(const CInvoice &x);
	bool delIssued(const CInvoice &x);
	bool delAccepted(const CInvoice &x);
	list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) const;
};

#ifndef __PROGTEST__
bool equalLists(const list<CInvoice> &a, const list<CInvoice> &b) {
	// todo
}

int main(void) {
	CVATRegister r;
	assert(r.registerCompany("first Company"));
	assert(r.registerCompany("Second     Company"));
	assert(r.registerCompany("ThirdCompany, Ltd."));
	assert(r.registerCompany("Third Company, Ltd."));
	assert(!r.registerCompany("Third Company, Ltd."));
	assert(!r.registerCompany(" Third  Company,  Ltd.  "));
	assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 20)));
	assert(r.addIssued(CInvoice(CDate(2000, 1, 2), "FirSt Company", "Second Company ", 200, 30)));
	assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 30)));
	assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
	assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
	assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "Second Company ", "First Company", 300, 30)));
	assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34)));
	assert(!r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
	assert(!r.addIssued(CInvoice(CDate(2000, 1, 4), "First Company", "First   Company", 200, 30)));
	assert(!r.addIssued(CInvoice(CDate(2000, 1, 4), "Another Company", "First   Company", 200, 30)));
	assert(equalLists(r.unmatched("First Company", CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, false).addKey(CSortOpt::BY_DATE, false)),
					  list<CInvoice>{
						  CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000)}));
	assert(equalLists(r.unmatched("First Company", CSortOpt().addKey(CSortOpt::BY_DATE, true).addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true)),
					  list<CInvoice>{
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000)}));
	assert(equalLists(r.unmatched("First Company", CSortOpt().addKey(CSortOpt::BY_VAT, true).addKey(CSortOpt::BY_AMOUNT, true).addKey(CSortOpt::BY_DATE, true).addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true)),
					  list<CInvoice>{
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000)}));
	assert(equalLists(r.unmatched("First Company", CSortOpt()),
					  list<CInvoice>{
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
						  CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000)}));
	assert(equalLists(r.unmatched("second company", CSortOpt().addKey(CSortOpt::BY_DATE, false)),
					  list<CInvoice>{
						  CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Third Company, Ltd.", "Second     Company", 400, 34.000000)}));
	assert(equalLists(r.unmatched("last company", CSortOpt().addKey(CSortOpt::BY_VAT, true)),
					  list<CInvoice>{}));
	assert(r.addAccepted(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company ", 200, 30)));
	assert(r.addAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
	assert(r.addAccepted(CInvoice(CDate(2000, 1, 1), "Second company ", "First Company", 300, 32)));
	assert(equalLists(r.unmatched("First Company", CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(CSortOpt::BY_DATE, true)),
					  list<CInvoice>{
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
	assert(!r.delIssued(CInvoice(CDate(2001, 1, 1), "First Company", "Second Company ", 200, 30)));
	assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "A First Company", "Second Company ", 200, 30)));
	assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Hand", 200, 30)));
	assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 1200, 30)));
	assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 200, 130)));
	assert(r.delIssued(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company", 200, 30)));
	assert(equalLists(r.unmatched("First Company", CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(CSortOpt::BY_DATE, true)),
					  list<CInvoice>{
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
	assert(r.delAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
	assert(equalLists(r.unmatched("First Company", CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(CSortOpt::BY_DATE, true)),
					  list<CInvoice>{
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
	assert(r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
	assert(equalLists(r.unmatched("First Company", CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(CSortOpt::BY_DATE, true)),
					  list<CInvoice>{
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
						  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */