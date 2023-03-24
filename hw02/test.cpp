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

	ssize_t findName(const string &name, const string &surname) const;
	ssize_t findEmail(const string &email) const;
	void addToVectors(const shared_ptr<SPerson> & newPerson);
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
								Find methods
===============================================================================
*/

ssize_t CPersonalAgenda::findName(const string & name, const string & surname) const {
	//change to binary search
	SPerson::SName finding = SPerson::SName(name, surname);
	for(auto iter = byName.begin(); iter != byName.end(); iter++) {
		if((*iter)->fullname == finding) {
			return byName.begin() - iter;
		}
	}
	return -1;
}

ssize_t CPersonalAgenda::findEmail(const string & email) const {
	for(auto iter = byEmail.begin(); iter != byEmail.end(); iter++) {
		if((*iter)->email == email) {
			return byEmail.begin() - iter;
		}
	}
	return -1;
}

/*
===============================================================================
							Adding methods
===============================================================================
*/

void CPersonalAgenda::addToVectors(const shared_ptr<SPerson> & newPerson) {
	//todo: something like binary search where to add? This is T(3n) at worst!
	//todo: AND I'M REPEATING MYSELF!!! accept pointer to comparator and the vector to add to?
	//first add to names
	bool inserted = false;
	for(auto iter = byName.begin(); iter != byName.end() && !inserted; iter++) {
		//compare the two names;
		if(!(newPerson->fullname < (*iter)->fullname)) {
			byName.insert(iter,newPerson);
			inserted = true;
		}
	}
	if(!inserted) {
		byName.push_back(newPerson);
	}
	inserted = false;
	//then add to emails
	for(auto iter = byEmail.begin(); iter != byEmail.end() && !inserted; iter++) {
		//compare the two names;
		if(!(newPerson->email.compare((*iter)->email) < 0)) {
			byEmail.insert(iter,newPerson);
			inserted = true;
		}
	}
	if(!inserted) {
		byEmail.push_back(newPerson);
	}
	inserted = false;
	//then add to salaries
	for(auto iter = bySalary.begin(); iter != bySalary.end() && !inserted; iter++) {
		//compare the two names;
		if(!(newPerson->salary < (*iter)->salary)) {
			bySalary.insert(iter,newPerson);
			inserted = true;
		}
	}
	if(!inserted) {
		bySalary.push_back(newPerson);
	}
}

bool CPersonalAgenda::add(const string &name, const string &surname, const string &email, unsigned int salary) {
	//todo: use shared ptr
	shared_ptr<SPerson> newPerson = make_shared<SPerson>(name, surname, email, salary);
	if(findEmail(email) != -1) {
		return false;
	}
	addToVectors(newPerson);
	return true;
}

/*
===============================================================================
							Deleting methods
===============================================================================
*/

bool CPersonalAgenda::del(const string &name, const string &surname) {
	return false;
}

bool CPersonalAgenda::del(const string &email) {
	return false;
}

/*
===============================================================================
								Setters
===============================================================================
*/

bool CPersonalAgenda::changeName(const string &email, const string &newName, const string &newSurname) {
	return false;
}

bool CPersonalAgenda::changeEmail(const string &name, const string &surname, const string &newEmail) {
	return false;
}

bool CPersonalAgenda::setSalary(const string &name, const string &surname, unsigned int salary) {
	return false;
}

bool CPersonalAgenda::setSalary(const string &email, unsigned int salary) {
	return false;
}

/*
===============================================================================
								Getters
===============================================================================
*/

unsigned int CPersonalAgenda::getSalary(const string &name, const string &surname) const {
	return 0;
}

unsigned int CPersonalAgenda::getSalary(const string &email) const {
	return 0;
}

bool CPersonalAgenda::getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const {
	return false;
}

bool CPersonalAgenda::getRank(const string &email, int &rankMin, int &rankMax) const {
	return false;
}

bool CPersonalAgenda::getFirst(string &outName, string &outSurname) const {
	return false;
}

bool CPersonalAgenda::getNext(const string &name, const string &surname, string &outName, string &outSurname) const {
	return false;
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
