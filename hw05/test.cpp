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

class CInvoice {
private:
	CDate m_date;
	string m_seller;
	string m_buyer;
	unsigned int m_amount;
	double m_vat;

	size_t m_order;

public:
	/// @brief Constructor of CInvoice
	/// @param date Date of the invoice
	/// @param seller The seller
	/// @param buyer The buyer
	/// @param amount The amount of money transfered
	/// @param vat The value added tax
	CInvoice(const CDate &date, const string &seller, const string &buyer, const unsigned int &amount, const double &vat)
		: m_date(date), m_seller(seller), m_buyer(buyer), m_amount(amount), m_vat(vat) {
		m_order = 0;
	}

	/// @brief Gets the order of the invoice
	/// @return The order of the invoice
	size_t getOrder() {
		return m_order;
	}

	/// @brief Sets the order of the invoice
	/// @param order The order to set
	void setOrder(const size_t &order) {
		m_order = order;
	}

	/// @brief Getter of date
	CDate date() const {
		return m_date;
	}
	/// @brief Getter of seller
	string seller() const {
		return m_seller;
	}
	/// @brief Getter of buyer
	string buyer() const {
		return m_buyer;
	}
	/// @brief Getter of amount
	unsigned int amount() const {
		return m_amount;
	}
	/// @brief Getter of valued added tax
	double vat() const {
		return m_vat;
	}

	/// @brief Compare two invoices whether they are equal
	/// @param other The other invoice
	/// @return True when equal, otherwise false
	bool operator==(const CInvoice &other) const {
		return m_date.compare(other.m_date) == 0 && m_buyer == other.m_buyer && m_seller == other.m_seller && m_amount == other.m_amount && m_vat == other.m_vat;
	}

	struct hashFunction {
		/// @brief Hashes the invoice
		/// @param invoice invoice to hash
		/// @return Hash of the invoice
		size_t operator()(const CInvoice &invoice) const {
			size_t dayHash = std::hash<int>()(invoice.m_date.day());
			size_t monthHash = std::hash<int>()(invoice.m_date.month()) << 1;
			size_t yearHash = std::hash<int>()(invoice.m_date.year()) << 2;

			size_t sellerHash = std::hash<string>()(invoice.m_seller) << 3;
			size_t buyerHash = std::hash<string>()(invoice.m_buyer) << 4;

			size_t amountHash = std::hash<unsigned int>()(invoice.m_amount) << 5;
			size_t vatHash = std::hash<double>()(invoice.m_vat) << 6;

			return dayHash ^ monthHash ^ yearHash ^ sellerHash ^ buyerHash ^ amountHash ^ vatHash;
		}
	};
};

class CCompany {
private:
	string m_originalName;
	string m_canonicalName;

	size_t m_invoiceCount;

	unordered_set<CInvoice, CInvoice::hashFunction> m_issued;
	unordered_set<CInvoice, CInvoice::hashFunction> m_accepted;

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

	/// @brief Adds an invoice to a given set of invoices, if not already contained
	/// @param invoice The invoice to add
	/// @param invoices The set where to add the invoice
	/// @return True when succesfully added, otherwise false
	bool add(CInvoice &invoice, unordered_set<CInvoice, CInvoice::hashFunction> &invoices) {
		if (invoices.count(invoice) != 0) {
			return false;
		}
		invoice.setOrder(m_invoiceCount++);
		invoices.insert(invoice);
		return true;
	}

public:
	/// @brief Construct a new CCompany object given by the name
	/// @param name Name of the company
	CCompany(const string &name) : m_originalName(name), m_canonicalName(toCanonical(name)) {
		m_invoiceCount = 0;
	}

	/// @brief Adds an invoice to the set of issued invoices, if not already contained
	/// @param invoice The invoice to add
	/// @return True when succesfully added, otherwise false
	bool addIssued(CInvoice &invoice) {
		return add(invoice, m_issued);
	}

	/// @brief Adds an invoice to the set of accepted invoices, if not already contained
	/// @param invoice The invoice to add
	/// @return True when succesfully added, otherwise false
	bool addAccepted(CInvoice &invoice) {
		return add(invoice, m_accepted);
	}

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

	/// @brief Getter of canonical name 
	string getCanonicalName() const {
		return m_canonicalName;
	}

	/// @brief Getter of original name 
	string getOriginalName() const {
		return m_originalName;
	}
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
	unordered_map<string, CCompany> m_companyRegister;

public:
	/// @brief Empty constructor of CVATRegister
	CVATRegister() : m_companyRegister() {}

	/// @brief Registers a company if it doesn't already exist
	/// @param name The name of the company
	/// @return True if succesfully added, otherwise false
	bool registerCompany(const string &name) {
		CCompany tempCompany(name);
		if (m_companyRegister.count(tempCompany.getCanonicalName()) != 0) {
			return false;
		}
		m_companyRegister.insert(make_pair(tempCompany.getCanonicalName(), tempCompany));
		return true;
	}

	bool delIssued(const CInvoice &x);
	bool delAccepted(const CInvoice &x);
	list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) const;
};

#ifndef __PROGTEST__
bool equalLists(const list<CInvoice> &a, const list<CInvoice> &b) {
	// todo
}

void testCCompany() {
	assert(CCompany("first Company") == CCompany("First Company"));
	assert(CCompany("Second     Company") == CCompany("Second Company "));
	assert(CCompany("ThirdCompany, Ltd.") != CCompany("Third Company, Ltd."));
	assert(CCompany("Third Company, Ltd.") == CCompany("Third Company, Ltd."));
	assert(CCompany("Third Company, Ltd.") == CCompany(" Third  Company,  Ltd.  "));
}

int main(void) {
	testCCompany();
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