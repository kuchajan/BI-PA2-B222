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
	struct SName {
		string name;
		string surname;
		SName(const string &nm, const string &srnm) {
			name = nm;
			surname = srnm;
		}
		friend bool operator==(const SName &first, const SName &other) {
			return (first.name == other.name) && (first.surname == other.surname);
		}
	};
	SName fullname;
	string email;
	unsigned int salary;
	SPerson(const string &nm, const string &srnm, const string &em, const unsigned int &sal)
		: fullname(nm, srnm) {
		email = em;
		salary = sal;
	}
#ifndef __PROGTEST__
	friend ostream &operator<<(ostream &os, SPerson person) {
		os << person.fullname.surname << ", " << person.fullname.name << "; " << person.email << "; " << person.salary;
		return os;
	}
#endif
};

class CPersonalAgenda {
private:
	vector<shared_ptr<SPerson>> byName;
	vector<shared_ptr<SPerson>> byEmail;
	vector<shared_ptr<SPerson>> bySalary;

	bool find(const SPerson & toFind, const vector<shared_ptr<SPerson>> & vec, size_t &idx, int (*comparator)(const SPerson &, const SPerson &)) const;

	void addToVector(const shared_ptr<SPerson> &newPerson, vector<shared_ptr<SPerson>> &vec, int (*comparator)(const SPerson &, const SPerson &));

	void setSalary(const shared_ptr<SPerson> &person, const unsigned int &salary);

	void getRank(const shared_ptr<SPerson> &person, int &rankMin, int &rankMax) const;

	void del(const size_t &idxName, const size_t &idxEmail);

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
#ifndef __PROGTEST__

	void printName() const {
		for (auto person : byName) {
			cout << *person << endl;
		}
	}

	void printEmail() const {
		for (auto person : byEmail) {
			cout << *person << endl;
		}
	}

	void printSalary() const {
		for (auto person : bySalary) {
			cout << *person << endl;
		}
	}

#endif
};

/*
===============================================================================
						Constructors and destructors
===============================================================================
*/

CPersonalAgenda::CPersonalAgenda(void) {} // nothing

CPersonalAgenda::~CPersonalAgenda(void) { // I guess I could empty the vectors
	byName.clear();
	byEmail.clear();
	bySalary.clear();
}

/*
===============================================================================
								Comparators
===============================================================================
*/

int cmpName(const SPerson &lhs, const SPerson &rhs) {
	if (lhs.fullname.surname == rhs.fullname.surname) {
		return lhs.fullname.name.compare(rhs.fullname.name);
	}
	return lhs.fullname.surname.compare(rhs.fullname.surname);
}

int cmpEmail(const SPerson &lhs, const SPerson &rhs) {
	return lhs.email.compare(rhs.email);
}

int cmpSalary(const SPerson &lhs, const SPerson &rhs) {
	return (lhs.salary > rhs.salary) - (lhs.salary < rhs.salary);
}

int cmpSalaryEmail(const SPerson &lhs, const SPerson &rhs) {
	if(lhs.salary == rhs.salary) {
		return cmpEmail(lhs,rhs);
	}
	return cmpSalary(lhs,rhs);
}

/*
===============================================================================
								Find methods
===============================================================================
*/

bool CPersonalAgenda::find(const SPerson & toFind, const vector<shared_ptr<SPerson>> & vec, size_t &idx, int (*comparator)(const SPerson &, const SPerson &)) const {
	if(vec.size() == 0) {
		return false;
	}
	size_t low = 0, high = vec.size() - 1;
	while (high - low > 0) {
		size_t middle = (high + low) / 2;
		int result = comparator(*(vec[middle]),toFind);
		if (result == 0) {
			idx = middle;
			return true;
		}
		if(result < 0) {
			low = middle + 1;
		}
		else {
			high = middle;
		}
	}
	if(comparator(*(vec[low]), toFind) == 0) {
		idx = low;
		return true;
	}
	return false;
}

/*
===============================================================================
							Adding methods
===============================================================================
*/

void CPersonalAgenda::addToVector(const shared_ptr<SPerson> &newPerson, vector<shared_ptr<SPerson>> &vec, int (*comparator)(const SPerson &, const SPerson &)) {
	//! This is T(n) at worst!
	// todo: something like binary search where to add?
	for (auto iter = vec.begin(); iter != vec.end(); iter++) {
		if (comparator((*newPerson), (**iter)) < 0) {
			vec.insert(iter, newPerson);
			return;
		}
	}
	vec.push_back(newPerson);
}

bool CPersonalAgenda::add(const string &name, const string &surname, const string &email, unsigned int salary) {
	shared_ptr<SPerson> newPerson = make_shared<SPerson>(name, surname, email, salary);
	size_t temp;
	if (find(*newPerson, byName, temp, cmpName) || find(*newPerson, byEmail, temp, cmpEmail)) {
		return false;
	}
	addToVector(newPerson, byName, cmpName);
	addToVector(newPerson, byEmail, cmpEmail);
	addToVector(newPerson, bySalary, cmpSalaryEmail);
	return true;
}

/*
===============================================================================
							Deleting methods
===============================================================================
*/
void CPersonalAgenda::del(const size_t &idxName, const size_t &idxEmail) {
	size_t idxSalary;
	if(!find(*(byName[idxName]),bySalary,idxSalary,cmpSalaryEmail)) {
		throw logic_error("void CPersonalAgenda::del: parsed in someone who isn't in bySalary vector");
	}

	byName.erase(byName.begin() + idxName);
	byEmail.erase(byEmail.begin() + idxEmail);
	bySalary.erase(bySalary.begin() + idxSalary);
}

bool CPersonalAgenda::del(const string &name, const string &surname) {
	size_t idxName;
	SPerson toFind(name,surname,"",0);
	if(!find(toFind,byName,idxName,cmpName)) {
		return false;
	}

	size_t idxEmail;
	if(!find(*(byName[idxName]),byEmail,idxEmail,cmpEmail)) {
		throw logic_error("bool CPersonalAgenda::del: parsed in someone who isn't in byEmail");
	}

	del(idxName, idxEmail);
	return true;
}

bool CPersonalAgenda::del(const string &email) {
	size_t idxEmail;
	SPerson toFind("","",email,0);
	if (!find(toFind,byEmail,idxEmail,cmpEmail)) {
		return false;
	}

	size_t idxName;
	if(!find(*(byEmail[idxEmail]),byName,idxName,cmpName)) {
		throw logic_error("bool CPersonalAgenda::del: parsed in someone who isn't in byName");
	}

	del(idxName, idxEmail);
	return true;
}

/*
===============================================================================
								Setters
===============================================================================
*/

bool CPersonalAgenda::changeName(const string &email, const string &newName, const string &newSurname) {
	// note to self: this is getting repetitive, am I doing this right?
	size_t idxEmail, idxName;
	SPerson toFind(newName,newSurname,email,0);
	// email not included or name is duplicite
	if (!find(toFind,byEmail,idxEmail,cmpEmail) || find(toFind,byName,idxName,cmpName)) {
		return false;
	}

	if (!find(*(byEmail[idxEmail]),byName,idxName,cmpName)) {
		throw logic_error("bool CPersonalAgenda::find: parsed in someone who isn't in byName");
	}

	byName.erase(byName.begin() + idxName);

	byEmail[idxEmail]->fullname = SPerson::SName(newName, newSurname);

	addToVector(byEmail[idxEmail], byName, cmpName);
	return true;
}

bool CPersonalAgenda::changeEmail(const string &name, const string &surname, const string &newEmail) {
	size_t idxName, idxEmail;
	SPerson toFind(name,surname,newEmail,0);
	// name not included or email is duplicite
	if(!find(toFind,byName,idxName,cmpName) || find(toFind,byEmail,idxEmail,cmpEmail)) {
		return false;
	}

	if(!find(*(byName[idxName]),byEmail,idxEmail,cmpEmail)) {
		throw logic_error("bool CPersonalAgenda::find: parsed in someone who isn't in byEmail");
	}

	byEmail.erase(byEmail.begin() + idxEmail);

	size_t idxSalary;
	if(!find(*(byName[idxName]),bySalary,idxSalary,cmpSalaryEmail)) {
		throw logic_error("bool CPersonalAgenda::find: parsed in someone who isn't in bySalary");
	}

	bySalary.erase(bySalary.begin() + idxSalary);

	byName[idxName]->email = newEmail;

	addToVector(byName[idxName], byEmail, cmpEmail);
	addToVector(byName[idxName], bySalary, cmpSalaryEmail);
	return true;
}

void CPersonalAgenda::setSalary(const shared_ptr<SPerson> &person, const unsigned int &salary) {
	size_t idxSalary;
	if(!find(*person,bySalary,idxSalary,cmpSalaryEmail)) {
		throw logic_error("void CPersonalAgenda::setSalary: parsed in someone who isn't in bySalary");
	}
	bySalary.erase(bySalary.begin() + idxSalary);
	person->salary = salary;
	addToVector(person, bySalary, cmpSalaryEmail);
}

bool CPersonalAgenda::setSalary(const string &name, const string &surname, unsigned int salary) {
	size_t idx;
	SPerson toFind(name,surname,"",0);
	if (!find(toFind,byName,idx,cmpName)) {
		return false;
	}
	setSalary(byName[idx], salary);
	return true;
}

bool CPersonalAgenda::setSalary(const string &email, unsigned int salary) {
	size_t idx;
	SPerson toFind("","",email,0);
	if (!find(toFind,byEmail,idx,cmpEmail)) {
		return false;
	}
	setSalary(byEmail[idx], salary);
	return true;
}

/*
===============================================================================
								Getters
===============================================================================
*/

unsigned int CPersonalAgenda::getSalary(const string &name, const string &surname) const {
	size_t idx;
	SPerson toFind(name,surname,"",0);
	if (!find(toFind,byName,idx,cmpName)) {
		return 0;
	}
	return byName[idx]->salary;
}

unsigned int CPersonalAgenda::getSalary(const string &email) const {
	size_t idx;
	SPerson toFind("","",email,0);
	if (!find(toFind,byEmail,idx,cmpEmail)) {
		return 0;
	}
	return byEmail[idx]->salary;
}

void CPersonalAgenda::getRank(const shared_ptr<SPerson> &person, int &rankMin, int &rankMax) const {
	size_t idx;
	if(!find(*person,bySalary,idx,cmpSalary)) {
		throw logic_error("void CPersonalAgenda::getRank: parsed in someone who hasn't got the same salary in bySalary");
	}

	//get lower bound
	size_t low = 0, high = idx;
	while(high - low > 0) {
		size_t middle = (high + low) / 2;
		int result = cmpSalary(*(bySalary[middle]),*person);
		if(result == 0) {
			rankMin = middle;
		}
		if(result < 0) {
			low = middle + 1;
		}
		else {
			high = middle;
		}
	}
	if(cmpSalary(*(bySalary[low]),*person) == 0) {
		rankMin = low;
	}

	//get upper bound
	low = idx, high = bySalary.size() - 1;
	while(high - low > 0) {
		size_t middle = (high + low) / 2;
		int result = cmpSalary(*(bySalary[middle]),*person);
		if(result == 0) {
			rankMax = middle;
		}
		if(result > 0) {
			high = middle;
		}
		else {
			low = middle + 1;
		}
	}
	if(cmpSalary(*(bySalary[low]),*person) == 0) {
		rankMax = low;
	}
}

bool CPersonalAgenda::getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const {
	size_t idx;
	SPerson toFind(name,surname,"",0);
	if(!find(toFind,byName,idx,cmpName)) {
		return false;
	}
	getRank(byName[idx], rankMin, rankMax);
	return true;
}

bool CPersonalAgenda::getRank(const string &email, int &rankMin, int &rankMax) const {
	size_t idx;
	SPerson toFind("","",email,0);
	if (!find(toFind,byEmail,idx,cmpEmail)) {
		return false;
	}
	getRank(byEmail[idx], rankMin, rankMax);
	return true;
}

bool CPersonalAgenda::getFirst(string &outName, string &outSurname) const {
	if (byName.empty()) {
		return false;
	}
	outName = byName[0]->fullname.name;
	outSurname = byName[0]->fullname.surname;
	return true;
}

bool CPersonalAgenda::getNext(const string &name, const string &surname, string &outName, string &outSurname) const {
	size_t idx;
	SPerson toFind(name,surname,"",0);
	if (!find(toFind,byName,idx,cmpName) || byName.size() == ++idx) {
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

	// # My tests #
	CPersonalAgenda b3;
	string tempName = outName, tempSurname = outSurname;

	assert(!b3.getFirst(outName, outSurname) && tempName == outName && tempSurname == outSurname);

	// 50 people
	assert(b3.add("John", "Cook", "cookjohn", 100'000));
	assert(b3.add("Caroline", "Holmes", "holmecar", 80'000));
	assert(b3.add("Peter", "Smith", "smithpet", 20'000));
	assert(b3.add("David", "Reed", "reeddavi", 39'000));
	assert(b3.add("Ashley", "Hunt", "huntashl", 42'000));
	assert(b3.add("Sarah", "Hardy", "hardysar", 20'000));
	assert(b3.add("James", "Bowen", "bowenjam", 74'000));
	assert(b3.add("Theresa", "Gallagher", "gallathe", 80'000));
	assert(b3.add("Oliver", "Hart", "hartoliv", 20'000));
	assert(b3.add("Andrew", "Tate", "colorofurlambo", 1));
	assert(b3.add("Michael", "Baird", "bairdmic", 20'000));
	assert(b3.add("Adam", "Robertson", "roberada", 39'000));
	assert(b3.add("Simon", "Singh", "singhsim", 100'000));
	assert(b3.add("Alice", "Gardiner", "gardiali", 25'000));
	assert(b3.add("Elizabeth", "Nelson", "nelsoeli", 25'000));
	assert(b3.add("Steve", "Miner", "minerste", 280'000));
	assert(b3.add("Alyx", "Crafter", "craftaly", 180'000));
	assert(b3.add("Stanley", "Hanson", "hansosta", 1'000'000));
	assert(b3.add("Samuel", "Rice", "ricesamu", 39'000));
	assert(b3.add("Max", "Payne", "paynemax", 28'000));
	assert(b3.add("Gordon", "Freeman", "freemgor", 950'000));
	assert(b3.add("Alyx", "Vance", "vancealy", 10));
	assert(b3.add("Sylvanas", "Windrunner", "windrsyl", 20));
	assert(b3.add("Arthas", "Menethil", "menetart", 1'000'000));
	assert(b3.add("Leon S", "Kennedy", "kenneleo", 25'000));
	assert(b3.add("Geralt", "Rivia", "riviager", 39'000));
	assert(b3.add("Solid", "Snake", "snakesol", 69'000));
	assert(b3.add("Lara", "Croft", "croftlar", 69'000));
	assert(b3.add("Donkey", "Kong", "kongdonk", 69'000));
	assert(b3.add("Duke", "Nukem", "chewasskickbubblegum", 69'000));
	assert(b3.add("Kazuma", "Kiryu", "kiryukaz", 550'000));
	assert(b3.add("William", "Afton", "aftonwil", 200'000));
	assert(b3.add("William", "Blazkowicz", "blazkwil", 39'000));
	assert(b3.add("Ladislav", "Vagner", "xvagner", UINT32_MAX));
	assert(b3.add("Deckard", "Cain", "caindeck", 200'000));
	assert(b3.add("Hajime", "Hinata", "hinathaj", 1'000));
	assert(b3.add("Cave", "Jonhson", "johnscav", 1'000'000));
	assert(b3.add("Tom", "Nook", "nooktom", 950'000));
	assert(b3.add("Junko", "Enoshima", "despairlover", 2'000));
	assert(b3.add("Tommy", "Angelo", "angeltom", 95'000));
	assert(b3.add("Vito", "Scaletta", "scalevit", 95'000));
	assert(b3.add("Sam Porter", "Bridges", "bridgsam", 38'000));
	assert(b3.add("Harry", "Potter", "pottehar", 950'000));
	assert(b3.add("JJ", "Maybank", "maybajj", 20));
	assert(b3.add("Daphne", "Bridgerton", "bridgdap", 450'000));
	assert(b3.add("Draco", "Malfoy", "malfodra", 700'000));
	assert(b3.add("Sirius", "Black", "blacksir", 100'000));
	assert(b3.add("Elizabeth", "Cooper", "coopeeli", 35'000));
	assert(b3.add("Melinda", "Springcloud", "sprinmel", 400));
	assert(b3.add("Five", "Hargreeves", "hargrfiv", 350'000));

	// add
	assert(!b3.add("John", "Cook", "cookjohn", 100'000));
	assert(!b3.add("John", "Cook", "whatisemail", 100'000));
	assert(!b3.add("Johnny", "Cook", "cookjohn", 100'000));

	assert(b3.add("Temp", "Cook", "cooktemp", 100'000));
	assert(b3.add("John", "Temp", "tempjohn", 100'000));

	// del
	assert(!b3.del("Nonexistant", "Cook"));
	assert(!b3.del("Nonexistant"));

	assert(b3.del("Temp", "Cook"));
	assert(b3.del("tempjohn"));

	// change
	assert(!b3.changeName("nonexistant", "Jan", "Novak"));
	assert(!b3.changeEmail("Non", "Existant", "coolemail"));

	assert(!b3.changeName("holmecar", "John", "Cook"));
	assert(!b3.changeEmail("John", "Cook", "colorofurlambo"));

	assert(b3.changeName("holmecar", "Caroline", "Cook"));
	assert(b3.changeEmail("Caroline", "Cook", "cookcaro"));

	// set salary
	assert(!b3.setSalary("nonexistant", 100));
	assert(!b3.setSalary("non", "existant", 100));

	assert(b3.setSalary("cookcaro", 1'000'000));
	assert(b3.setSalary("David", "Reed", 20'000));

	// get salary
	assert(b3.getSalary("nonexistant") == 0);
	assert(b3.getSalary("Non", "Existant") == 0);

	assert(b3.getSalary("cookcaro") == 1'000'000);
	assert(b3.getSalary("David", "Reed") == 20'000);

	// get rank
	assert(b3.getRank("xvagner", lo, hi) && lo == 49 && hi == 49);
	assert(b3.getRank("Andrew", "Tate", lo, hi) && lo == 0 && hi == 0);

	assert(!b3.getRank("nonexistant", lo, hi) && lo == 0 && hi == 0);
	assert(!b3.getRank("Non", "Existant", lo, hi) && lo == 0 && hi == 0);

	// get first
	assert(b3.getFirst(outName, outSurname) && outName == "William" && outSurname == "Afton");

	// get next
	assert(!b3.getNext("Non", "Existant", outName, outSurname) && outName == "William" && outSurname == "Afton");
	assert(!b3.getNext("Sylvanas", "Windrunner", outName, outSurname) && outName == "William" && outSurname == "Afton");

	cout << "By name:" << endl;
	b3.printName();

	cout << "\nBy email:" << endl;
	b3.printEmail();

	cout << "\nBySalary:" << endl;
	b3.printSalary();

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
