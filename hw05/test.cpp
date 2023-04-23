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

class CInvoice {
private:
	CDate m_date;

	string m_sellerOriginal;
	string m_sellerCanonical;

	string m_buyerOriginal;
	string m_buyerCanonical;

	unsigned int m_amount;
	double m_vat;

	size_t m_order;

	bool wasIssued;
	bool wasAccepted;

public:
	/// @brief Constructor of CInvoice
	/// @param date Date of the invoice
	/// @param seller The seller
	/// @param buyer The buyer
	/// @param amount The amount of money transfered
	/// @param vat The value added tax
	CInvoice(const CDate &date, const string &seller, const string &buyer, const unsigned int &amount, const double &vat)
		: m_date(date), m_sellerCanonical(toCanonical(seller)), m_buyerCanonical(toCanonical(buyer)), m_amount(amount), m_vat(vat) {
		m_order = 0;
		m_sellerOriginal = "";
		m_buyerOriginal = "";
		wasIssued = false;
		wasAccepted = false;
	}

	bool getWasIssued() const {
		return wasIssued;
	}

	void setWasIssued(bool setTo) {
		wasIssued = setTo;
	}

	bool getWasAccepted() const {
		return wasAccepted;
	}

	void setWasAccepted(bool setTo) {
		wasAccepted = setTo;
	}

	/// @brief Set the original name of the selling company
	/// @param sellerOriginal The original name of the selling company
	void setSellerOriginal(const string &sellerOriginal) {
		m_sellerOriginal = string(sellerOriginal);
	}

	/// @brief Set the original name of the buying company
	/// @param sellerOriginal The original name of the buying company
	void setBuyerOriginal(const string &buyerOriginal) {
		m_buyerOriginal = string(buyerOriginal);
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
	/// @brief Getter of original name of seller
	string seller() const {
		return m_sellerOriginal;
	}
	/// @brief Getter of canonical name of seller
	string getSellerCanonical() const {
		return m_sellerCanonical;
	}

	/// @brief Getter of original name of buyer
	string buyer() const {
		return m_buyerOriginal;
	}
	/// @brief Getter of canonical name of buyer
	string getBuyerCanonical() const {
		return m_buyerCanonical;
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
		return m_date.compare(other.m_date) == 0 && m_buyerCanonical == other.m_buyerCanonical && m_sellerCanonical == other.m_sellerCanonical && m_amount == other.m_amount && m_vat == other.m_vat;
	}

	int normalizeCmpRes(const int &res) const {
		return res < 0 ? -1 : (res == 0 ? 0 : 1);
	}

	int cmpDate(const CInvoice &rhs) const {
		return normalizeCmpRes(m_date.compare(rhs.m_date));
	}

	int cmpSeller(const CInvoice &rhs) const {
		return normalizeCmpRes(m_sellerOriginal.compare(rhs.m_sellerOriginal));
	}

	int cmpBuyer(const CInvoice &rhs) const {
		return normalizeCmpRes(m_buyerOriginal.compare(rhs.m_buyerOriginal));
	}

	int cmpAmount(const CInvoice &rhs) const {
		return m_amount < rhs.m_amount ? -1 : (m_amount == rhs.m_amount ? 0 : 1);
	}

	int cmpVAT(const CInvoice &rhs) const {
		return m_vat < rhs.m_vat ? -1 : (m_vat == rhs.m_vat ? 0 : 1);
	}

	int cmpOrder(const CInvoice &rhs) const {
		return m_order < rhs.m_order ? -1 : 1; // orders can never be same (comparing orders of invoices from different companies is non-sensical)
	}

	struct hashFunction {
		/// @brief Hashes the invoice
		/// @param invoice invoice to hash
		/// @return Hash of the invoice
		size_t operator()(const CInvoice &invoice) const {
			size_t dayHash = std::hash<int>()(invoice.m_date.day());
			size_t monthHash = std::hash<int>()(invoice.m_date.month()) << 1;
			size_t yearHash = std::hash<int>()(invoice.m_date.year()) << 2;

			size_t sellerHash = std::hash<string>()(invoice.m_sellerCanonical) << 3;
			size_t buyerHash = std::hash<string>()(invoice.m_buyerCanonical) << 4;

			size_t amountHash = std::hash<unsigned int>()(invoice.m_amount) << 5;
			size_t vatHash = std::hash<double>()(invoice.m_vat) << 6;

			return dayHash ^ monthHash ^ yearHash ^ sellerHash ^ buyerHash ^ amountHash ^ vatHash;
		}
	};
};

class CSortOpt {
private:
	vector<pair<int, bool>> m_keys;

	/// @brief
	/// @param lhs
	/// @param rhs
	/// @param key
	/// @return
	int compareByKey(const CInvoice &lhs, const CInvoice &rhs, const pair<int, bool> &key) const {
		int res = 0;
		switch (key.first) {
			case BY_DATE: {
				res = lhs.cmpDate(rhs);
				break;
			}
			case BY_BUYER: {
				res = lhs.cmpBuyer(rhs);
				break;
			}
			case BY_SELLER: {
				res = lhs.cmpSeller(rhs);
				break;
			}
			case BY_AMOUNT: {
				res = lhs.cmpAmount(rhs);
				break;
			}
			case BY_VAT: {
				res = lhs.cmpVAT(rhs);
				break;
			}
			default: {
				throw invalid_argument("Tried to sort with a wrong key value");
				break;
			}
		}
		// invert if not ascending
		return key.second ? res : res * (-1);
	}

	/// @brief
	/// @param lhs
	/// @param rhs
	/// @return
	int compare(const CInvoice &lhs, const CInvoice &rhs) const {
		for (auto key : m_keys) {
			int res = compareByKey(lhs, rhs, key);
			if (res != 0) {
				return res;
			}
		}
		return lhs.cmpOrder(rhs);
	}

public:
	// note: I would personally prefer an enum instead of this, but the Gods of ProgTest have spoken and so it shall be
	static const int BY_DATE = 0;
	static const int BY_BUYER = 1;
	static const int BY_SELLER = 2;
	static const int BY_AMOUNT = 3;
	static const int BY_VAT = 4;

	/// @brief Empty constructor
	CSortOpt() {}

	/// @brief Adds a key which to sort by
	/// @param key Key to add
	/// @param ascending whether to sort should be ascending or not (descending)
	/// @return This object (to chain methods)
	CSortOpt &addKey(int key, bool ascending = true) {
		if (key < 0 || key > 4) {
			throw invalid_argument("Tried to insert an invalid key.");
		}
		m_keys.push_back(make_pair(key, ascending));
		return *this;
	}

	bool operator()(const CInvoice &lhs, const CInvoice &rhs) const {
		return compare(lhs, rhs) < 0;
	}
};

class CCompany {
private:
	string m_originalName;
	string m_canonicalName;

	unordered_map<CInvoice, shared_ptr<CInvoice>, CInvoice::hashFunction> m_invoices;

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
	CCompany(const string &name, const string &canonicalName) : m_originalName(name), m_canonicalName(canonicalName) {}

	unordered_map<CInvoice, shared_ptr<CInvoice>, CInvoice::hashFunction>::iterator find(const CInvoice &invoice, bool &success) {
		auto iter = m_invoices.find(invoice);
		success = iter != m_invoices.end();
		return iter;
	}

	/// @brief Adds an invoice to a given set of invoices, if not already contained
	/// @param invoice The invoice to add
	/// @param invoices The set where to add the invoice
	/// @return True when succesfully added, otherwise false
	bool add(shared_ptr<CInvoice> invoice, bool issuing) {
		// Check if it exists already
		auto iter = m_invoices.find(*invoice); // Through the find function, since we then do more operations on it if it exists
		if (iter != m_invoices.end()) {
			// Check if it was issued / accepted already
			if (issuing ? (*((*iter).second)).getWasIssued() : (*((*iter).second)).getWasAccepted()) {
				return false;
			}
			// Set that it was issued / accepted now
			issuing ? (*((*iter).second)).setWasIssued(true) : (*((*iter).second)).setWasAccepted(true);
			return true;
		}

		// Add it, since it doesn't exist yet
		m_invoices.insert(make_pair(*invoice, invoice));
		return true;
	}

	bool del(shared_ptr<CInvoice> invoice, bool issuing, bool &deleteInOther) {
		// we already know that the invoice exists
		// we don't know if it was already deleted from the issue / accept
		if (issuing ? !invoice->getWasIssued() : !invoice->getWasAccepted()) {
			return false;
		}
		// okay, it wasn't, set the issue / accept
		issuing ? invoice->setWasIssued(false) : invoice->setWasAccepted(false);
		// we just need to check if we need to get rid of it
		if (!invoice->getWasIssued() && !invoice->getWasAccepted()) {
			deleteInvoice(invoice);
			deleteInOther = true;
		}
		return true;
	}

	void deleteInvoice(const shared_ptr<CInvoice> &invoice) {
		m_invoices.erase(*invoice);
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

	vector<CInvoice> getUnmatchedInvoices(const CSortOpt &sortOpt) const {
		vector<CInvoice> invoices;

		// copy the invoices that are not issued or are not accepted
		for (auto iter = m_invoices.begin(); iter != m_invoices.end(); ++iter) {
			if ((*(*iter).second).getWasIssued() != true && (*(*iter).second).getWasAccepted() != true) {
				invoices.push_back(*(*iter).second);
			}
		}

		// sort them
		sort(invoices.begin(), invoices.end(), sortOpt);

		return invoices;
	}
};

class CVATRegister {
private:
	unordered_map<string, CCompany> m_companyRegister;

	size_t m_invoiceCount;

	/// @brief Finds two companies from the given invoice, returns their iterators and whether they were found
	/// @param[in,out] x The invoice to get the companies from (names are overwritten by their canonical name)
	/// @param[out] iSeller Iterator to the selling company
	/// @param[out] iBuyer Iterator to the buying company
	/// @return True when found, otherwise false
	bool findCompaniesFromInvoice(CInvoice &x, std::unordered_map<std::string, CCompany>::iterator &iSeller, std::unordered_map<std::string, CCompany>::iterator &iBuyer) {
		if (x.getSellerCanonical() == x.getBuyerCanonical()) {
			return false;
		}
		if ((iSeller = m_companyRegister.find(x.getSellerCanonical())) == m_companyRegister.end()) {
			return false;
		}
		if ((iBuyer = m_companyRegister.find(x.getBuyerCanonical())) == m_companyRegister.end()) {
			return false;
		}

		x.setSellerOriginal((*iSeller).second.getOriginalName());
		x.setBuyerOriginal((*iBuyer).second.getOriginalName());
		return true;
	}

	/// @brief Attempts to add an invoice to a list of issued invoices of a selling company from the invoice
	/// @param x The invoice to add
	/// @return True when succesfully added, otherwise false
	bool add(CInvoice x, bool issuing) {
		std::unordered_map<std::string, CCompany>::iterator iSeller, iBuyer;
		if (!findCompaniesFromInvoice(x, iSeller, iBuyer)) {
			return false;
		}

		bool success = false;
		auto iter = (*iSeller).second.find(x, success);

		if (!success) {
			// if it doesn't exist in the first one, it surely doesn't in the other one
			shared_ptr<CInvoice> toAdd = make_shared<CInvoice>(x);
			toAdd->setOrder(m_invoiceCount++);
			issuing ? toAdd->setWasIssued(true) : toAdd->setWasAccepted(true);
			// no reasone to fail
			(*iSeller).second.add(toAdd, issuing);
			(*iBuyer).second.add(toAdd, issuing);
			return true;
		}
		return (*iSeller).second.add((*iter).second, issuing);
	}

	bool del(CInvoice x, bool issuing) {
		std::unordered_map<std::string, CCompany>::iterator iSeller, iBuyer;
		if (!findCompaniesFromInvoice(x, iSeller, iBuyer)) {
			return false;
		}

		bool success = false;
		auto iter = iSeller->second.find(x, success);

		if (!success) {
			return false;
		}

		bool deleteInOther = false;

		if (!iSeller->second.del(iter->second, issuing, deleteInOther)) {
			return false;
		}

		if (deleteInOther) {
			auto iter2 = iBuyer->second.find(x, success);
			iBuyer->second.deleteInvoice(iter2->second);
		}
		return true;
	}

public:
	/// @brief Empty constructor of CVATRegister
	CVATRegister() : m_companyRegister() {}

	/// @brief Registers a company if it doesn't already exist
	/// @param name The name of the company
	/// @return True if succesfully added, otherwise false
	bool registerCompany(const string &name) {
		string canonical = toCanonical(name);
		if (m_companyRegister.count(canonical) != 0) {
			return false;
		}
		m_companyRegister.emplace(make_pair(canonical, CCompany(name, canonical)));
		return true;
	}

	/// @brief Attempts to add an invoice to a list of issued invoices of a selling company from the invoice
	/// @param x The invoice to add
	/// @return True when succesfully added, otherwise false
	bool addIssued(const CInvoice &x) {
		return add(x, true);
	}

	/// @brief Attempts to add an invoice to a list of accepted invoices of a buying company from the invoice
	/// @param x The invoice to add
	/// @return True when succesfully added, otherwise false
	bool addAccepted(const CInvoice &x) {
		return add(x, false);
	}

	bool delIssued(const CInvoice &x) {
		return del(x, true);
	}
	bool delAccepted(const CInvoice &x) {
		return del(x, false);
	}

	list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) const {
		string canonical = toCanonical(company);
		auto iCompany = m_companyRegister.find(canonical);
		if (iCompany == m_companyRegister.end()) {
			return list<CInvoice>();
		}

		vector<CInvoice> invoices = (*iCompany).second.getUnmatchedInvoices(sortBy);

		return list<CInvoice>(invoices.begin(), invoices.end());
	}
};

#ifndef __PROGTEST__
bool equalLists(const list<CInvoice> &a, const list<CInvoice> &b) {
	return equal(a.cbegin(), a.cend(), b.cbegin());
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