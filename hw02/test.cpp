#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>
using namespace std;
#endif /* __PROGTEST__ */

/*
===============================================================================
				Declarations of classes, structures, and methods
===============================================================================
*/

struct SPerson {
	struct SName
	{
		string name;
		string surname;
		SName(const string & nm, const string & srnm) {
			name = nm;
			surname = srnm;
		}
		friend bool operator== (const SName & first, const SName & other) {
			return (first.name == other.name) && (first.surname == other.surname); 
		}
		friend bool operator< (const SName & first, const SName & other) {
			if(first.surname != other.surname) {
				return first.surname.compare(other.surname) < 0;
			}
			return first.name.compare(other.name) < 0;
		}
	};
	SName fullname;
	string email;
	unsigned int salary;
	SPerson(const string & nm, const string & srnm, const string & em, const unsigned int & sal)
		: fullname(nm,srnm) {
		email = em;
		salary = sal;
	}
};

class CPersonalAgenda {
private:
	vector<shared_ptr<SPerson>> byName;
	vector<shared_ptr<SPerson>> byEmail;
	vector<shared_ptr<SPerson>> bySalary;

	bool findName(const string &name, const string &surname, size_t & idx) const;
	bool findEmail(const string &email, size_t & idx) const;
	size_t findSalary(const string &email, const unsigned int & salary) const;

	void addToVector(const shared_ptr<SPerson> & newPerson, vector<shared_ptr<SPerson>> & vec, bool (*comparator)(const SPerson &, const SPerson &));

	void setSalary(const shared_ptr<SPerson> & person, const unsigned int & salary);

	void getRank(const shared_ptr<SPerson> & person, int &rankMin, int &rankMax) const;

	void del(const size_t & idxName, const size_t & idxEmail);
public:
	CPersonalAgenda(void);
	~CPersonalAgenda(void);
	bool add(const string &name,
			 const string &surname,
			 const string &email,
			 unsigned int salary);
	bool del(const string &name,
			 const string &surname);
	bool del(const string &email);
	bool changeName(const string &email,
					const string &newName,
					const string &newSurname);
	bool changeEmail(const string &name,
					 const string &surname,
					 const string &newEmail);
	bool setSalary(const string &name,
				   const string &surname,
				   unsigned int salary);
	bool setSalary(const string &email,
				   unsigned int salary);
	unsigned int getSalary(const string &name,
						   const string &surname) const;
	unsigned int getSalary(const string &email) const;
	bool getRank(const string &name,
				 const string &surname,
				 int &rankMin,
				 int &rankMax) const;
	bool getRank(const string &email,
				 int &rankMin,
				 int &rankMax) const;
	bool getFirst(string &outName,
				  string &outSurname) const;
	bool getNext(const string &name,
				 const string &surname,
				 string &outName,
				 string &outSurname) const;
};

/*
===============================================================================
						Constructors and destructors
===============================================================================
*/

CPersonalAgenda::CPersonalAgenda(void) { } //nothing

CPersonalAgenda::~CPersonalAgenda(void) { //I guess I could empty the vectors
	byName.clear();
	byEmail.clear();
	bySalary.clear();
}

/*
===============================================================================
								Comparators
===============================================================================
*/

bool cmpName(const SPerson & lhs, const SPerson & rhs) {
	//(newPerson->fullname < (*iter)->fullname)
	return (lhs.fullname < rhs.fullname);
}

bool cmpEmail(const SPerson & lhs, const SPerson & rhs) {
	//newPerson->email.compare((*iter)->email) < 0
	return (lhs.email.compare(rhs.email) < 0);
}

bool cmpSalaryEmail(const SPerson & lhs, const SPerson & rhs) {
	//((newPerson->salary == (*iter)->salary) && (newPerson->email.compare((*iter)->email) < 0)) || (newPerson->salary < (*iter)->salary)
	return ((lhs.salary == rhs.salary) && cmpEmail(lhs,rhs)) || (lhs.salary < rhs.salary);
}

/*
===============================================================================
								Find methods
===============================================================================
*/

//todo: binary search

bool CPersonalAgenda::findName(const string & name, const string & surname, size_t & idx) const {
	//change to binary search
	SPerson::SName finding = SPerson::SName(name, surname);
	for(auto iter = byName.begin(); iter != byName.end(); iter++) {
		if((*iter)->fullname == finding) {
			idx = iter - byName.begin();
			return true;
		}
	}
	return false;
}

bool CPersonalAgenda::findEmail(const string & email, size_t & idx) const {
	for(auto iter = byEmail.begin(); iter != byEmail.end(); iter++) {
		if((*iter)->email == email) {
			idx = iter - byEmail.begin();
			return true;
		}
	}
	return false;
}

size_t CPersonalAgenda::findSalary(const string &email, const unsigned int &salary) const {
	for(auto iter = bySalary.begin(); iter != bySalary.end(); iter++) {
		if((*iter)->email == email) {
			return iter - bySalary.begin();
		}
	}
	throw logic_error ("CPersonalAgenda::findSalary: email " + email + " not found (even though it should be contained in the DB)");
}

/*
===============================================================================
							Adding methods
===============================================================================
*/

void CPersonalAgenda::addToVector(const shared_ptr<SPerson> &newPerson, vector<shared_ptr<SPerson>> &vec, bool (*comparator)(const SPerson &, const SPerson &)) {
	//! This is T(n) at worst!
	//todo: something like binary search where to add?
	bool inserted = false;
	for(auto iter = vec.begin(); iter != vec.end() && !inserted; iter++) {
		if(comparator((*newPerson),(**iter))) {
			vec.insert(iter,newPerson);
			inserted = true;
		}
	}
	if(!inserted) {
		vec.push_back(newPerson);
	}
}

bool CPersonalAgenda::add(const string &name, const string &surname, const string &email, unsigned int salary) {
	shared_ptr<SPerson> newPerson = make_shared<SPerson>(name, surname, email, salary);
	size_t temp;
	if(findEmail(email, temp)) {
		return false;
	}
	addToVector(newPerson,byName,cmpName);
	addToVector(newPerson,byEmail,cmpEmail);
	addToVector(newPerson,bySalary,cmpSalaryEmail);
	return true;
}

/*
===============================================================================
							Deleting methods
===============================================================================
*/
void CPersonalAgenda::del(const size_t & idxName, const size_t & idxEmail) {
	size_t idxSalary = findSalary(byName[idxName]->email,byName[idxName]->salary);

	byName.erase(byName.begin()+idxName);
	byEmail.erase(byEmail.begin()+idxEmail);
	bySalary.erase(bySalary.begin()+idxSalary);
}

bool CPersonalAgenda::del(const string &name, const string &surname) {
	size_t idxName;
	if(!findName(name,surname,idxName)) {
		return false;
	}

	size_t idxEmail;
	findEmail(byName[idxName]->email,idxEmail);

	del(idxName,idxEmail);
	return true;
}

bool CPersonalAgenda::del(const string &email) {
	size_t idxEmail;
	if(!findEmail(email,idxEmail)) {
		return false;
	}

	size_t idxName;
	findName(byEmail[idxEmail]->fullname.name,byEmail[idxEmail]->fullname.surname,idxName);

	del(idxName,idxEmail);
	return true;
}

/*
===============================================================================
								Setters
===============================================================================
*/

bool CPersonalAgenda::changeName(const string &email, const string &newName, const string &newSurname) {
	//note to self: this is getting repetitive, am I doing this right?
	size_t idxEmail;
	if(!findEmail(email,idxEmail)) {
		return false;
	}

	size_t idxName;
	findName(byEmail[idxEmail]->fullname.name, byEmail[idxEmail]->fullname.surname, idxName);

	byName.erase(byName.begin()+idxName);

	byEmail[idxEmail]->fullname = SPerson::SName(newName,newSurname);

	addToVector(byEmail[idxEmail],byName,cmpName);
	return true;
}

bool CPersonalAgenda::changeEmail(const string &name, const string &surname, const string &newEmail) {
	size_t idxName;
	if(!findName(name, surname, idxName)) {
		return false;
	}

	size_t idxEmail;
	findEmail(byName[idxName]->email,idxEmail);

	byEmail.erase(byEmail.begin()+idxEmail);

	size_t idxSalary = findSalary(byName[idxName]->email,byName[idxName]->salary);
	bySalary.erase(bySalary.begin()+idxSalary);

	byName[idxName]->email = newEmail;

	addToVector(byName[idxName], byEmail, cmpEmail);
	addToVector(byName[idxName], bySalary, cmpSalaryEmail);
	return true;
}

void CPersonalAgenda::setSalary(const shared_ptr<SPerson> &person, const unsigned int &salary) {
	bySalary.erase(bySalary.begin() + findSalary(person->email,person->salary));
	person->salary = salary;
	addToVector(person,bySalary,cmpSalaryEmail);
}

bool CPersonalAgenda::setSalary(const string &name, const string &surname, unsigned int salary) {
	size_t idx;
	if(!findName(name,surname,idx)) {
		return false;
	}
	setSalary(byName[idx],salary);
	return true;
}

bool CPersonalAgenda::setSalary(const string &email, unsigned int salary) {
	size_t idx;
	if(!findEmail(email,idx)) {
		return false;
	}
	setSalary(byEmail[idx],salary);
	return true;
}

/*
===============================================================================
								Getters
===============================================================================
*/

unsigned int CPersonalAgenda::getSalary(const string &name, const string &surname) const {
	size_t idx;
	if(!findName(name,surname,idx)) {
		return 0;
	}
	return byName[idx]->salary;
}

unsigned int CPersonalAgenda::getSalary(const string &email) const {
	size_t idx;
	if(!findEmail(email,idx)) {
		return 0;
	}
	return byEmail[idx]->salary;
}

void CPersonalAgenda::getRank(const shared_ptr<SPerson> & person, int &rankMin, int &rankMax) const {
	size_t idx = findSalary(person->email,person->salary);
	
	//todo: binary search
	for(rankMin = idx; rankMin != 0 && bySalary[rankMin-1]->salary == person->salary; rankMin--) { }
	for(rankMax = idx; rankMax+1 != (int) bySalary.size() && bySalary[rankMax+1]->salary == person->salary; rankMax++) { }
}

bool CPersonalAgenda::getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const {
	size_t idx;
	if(!findName(name,surname,idx)) {
		return false;
	}
	getRank(byName[idx],rankMin,rankMax);
	return true;
}

bool CPersonalAgenda::getRank(const string &email, int &rankMin, int &rankMax) const {
	size_t idx;
	if(!findEmail(email,idx)) {
		return false;
	}
	getRank(byEmail[idx],rankMin,rankMax);
	return true;
}

bool CPersonalAgenda::getFirst(string &outName, string &outSurname) const {
	if(byName.empty()) {
		return false;
	}
	outName = byName[0]->fullname.name;
	outSurname = byName[0]->fullname.surname;
	return true;
}

bool CPersonalAgenda::getNext(const string &name, const string &surname, string &outName, string &outSurname) const {
	size_t idx;
	if(!findName(name,surname,idx) || byName.size() == ++idx) {
		return false;
	}
	outName = byName[idx]->fullname.name;
	outSurname = byName[idx]->fullname.surname;
	return true;
}

/*
===============================================================================
								Tests
===============================================================================
*/

#ifndef __PROGTEST__
int main(void) {
	string outName, outSurname;
	int lo, hi;

	CPersonalAgenda b1;
	assert(b1.add("John", "Smith", "john", 30000));
	assert(b1.add("John", "Miller", "johnm", 35000));
	assert(b1.add("Peter", "Smith", "peter", 23000));
	assert(b1.getFirst(outName, outSurname) && outName == "John" && outSurname == "Miller");
	assert(b1.getNext("John", "Miller", outName, outSurname) && outName == "John" && outSurname == "Smith");
	assert(b1.getNext("John", "Smith", outName, outSurname) && outName == "Peter" && outSurname == "Smith");
	assert(!b1.getNext("Peter", "Smith", outName, outSurname));
	assert(b1.setSalary("john", 32000));
	assert(b1.getSalary("john") == 32000);
	assert(b1.getSalary("John", "Smith") == 32000);
	assert(b1.getRank("John", "Smith", lo, hi) && lo == 1 && hi == 1);
	assert(b1.getRank("john", lo, hi) && lo == 1 && hi == 1);
	assert(b1.getRank("peter", lo, hi) && lo == 0 && hi == 0);
	assert(b1.getRank("johnm", lo, hi) && lo == 2 && hi == 2);
	assert(b1.setSalary("John", "Smith", 35000));
	assert(b1.getSalary("John", "Smith") == 35000);
	assert(b1.getSalary("john") == 35000);
	assert(b1.getRank("John", "Smith", lo, hi) && lo == 1 && hi == 2);
	assert(b1.getRank("john", lo, hi) && lo == 1 && hi == 2);
	assert(b1.getRank("peter", lo, hi) && lo == 0 && hi == 0);
	assert(b1.getRank("johnm", lo, hi) && lo == 1 && hi == 2);
	assert(b1.changeName("peter", "James", "Bond"));
	assert(b1.getSalary("peter") == 23000);
	assert(b1.getSalary("James", "Bond") == 23000);
	assert(b1.getSalary("Peter", "Smith") == 0);
	assert(b1.getFirst(outName, outSurname) && outName == "James" && outSurname == "Bond");
	assert(b1.getNext("James", "Bond", outName, outSurname) && outName == "John" && outSurname == "Miller");
	assert(b1.getNext("John", "Miller", outName, outSurname) && outName == "John" && outSurname == "Smith");
	assert(!b1.getNext("John", "Smith", outName, outSurname));
	assert(b1.changeEmail("James", "Bond", "james"));
	assert(b1.getSalary("James", "Bond") == 23000);
	assert(b1.getSalary("james") == 23000);
	assert(b1.getSalary("peter") == 0);
	assert(b1.del("james"));
	assert(b1.getRank("john", lo, hi) && lo == 0 && hi == 1);
	assert(b1.del("John", "Miller"));
	assert(b1.getRank("john", lo, hi) && lo == 0 && hi == 0);
	assert(b1.getFirst(outName, outSurname) && outName == "John" && outSurname == "Smith");
	assert(!b1.getNext("John", "Smith", outName, outSurname));
	assert(b1.del("john"));
	assert(!b1.getFirst(outName, outSurname));
	assert(b1.add("John", "Smith", "john", 31000));
	assert(b1.add("john", "Smith", "joHn", 31000));
	assert(b1.add("John", "smith", "jOhn", 31000));

	CPersonalAgenda b2;
	assert(!b2.getFirst(outName, outSurname));
	assert(b2.add("James", "Bond", "james", 70000));
	assert(b2.add("James", "Smith", "james2", 30000));
	assert(b2.add("Peter", "Smith", "peter", 40000));
	assert(!b2.add("James", "Bond", "james3", 60000));
	assert(!b2.add("Peter", "Bond", "peter", 50000));
	assert(!b2.changeName("joe", "Joe", "Black"));
	assert(!b2.changeEmail("Joe", "Black", "joe"));
	assert(!b2.setSalary("Joe", "Black", 90000));
	assert(!b2.setSalary("joe", 90000));
	assert(b2.getSalary("Joe", "Black") == 0);
	assert(b2.getSalary("joe") == 0);
	assert(!b2.getRank("Joe", "Black", lo, hi));
	assert(!b2.getRank("joe", lo, hi));
	assert(!b2.changeName("joe", "Joe", "Black"));
	assert(!b2.changeEmail("Joe", "Black", "joe"));
	assert(!b2.del("Joe", "Black"));
	assert(!b2.del("joe"));
	assert(!b2.changeName("james2", "James", "Bond"));
	assert(!b2.changeEmail("Peter", "Smith", "james"));
	assert(!b2.changeName("peter", "Peter", "Smith"));
	assert(!b2.changeEmail("Peter", "Smith", "peter"));
	assert(b2.del("Peter", "Smith"));
	assert(!b2.changeEmail("Peter", "Smith", "peter2"));
	assert(!b2.setSalary("Peter", "Smith", 35000));
	assert(b2.getSalary("Peter", "Smith") == 0);
	assert(!b2.getRank("Peter", "Smith", lo, hi));
	assert(!b2.changeName("peter", "Peter", "Falcon"));
	assert(!b2.setSalary("peter", 37000));
	assert(b2.getSalary("peter") == 0);
	assert(!b2.getRank("peter", lo, hi));
	assert(!b2.del("Peter", "Smith"));
	assert(!b2.del("peter"));
	assert(b2.add("Peter", "Smith", "peter", 40000));
	assert(b2.getSalary("peter") == 40000);

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
