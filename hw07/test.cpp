#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
#endif /* __PROGTEST__ */

template <typename M_>
class CContestant {
private:
	string m_name;
	map<string, M_> m_matches;

public:
	CContestant(const string &name) : m_name(name) {}

	/// @brief Finds out if other name is already contained
	bool contains(const string &otherName) const {
		return m_matches.find(otherName) != m_matches.end();
	}

	void addMatch(const string &otherName, const M_ &result) {
		m_matches.insert(make_pair(otherName, result));
	}

	typename map<string, M_>::const_iterator begin() const {
		return m_matches.cbegin();
	}
	typename map<string, M_>::const_iterator end() const {
		return m_matches.cend();
	}
};

template <typename M_>
class CContest {
private:
	map<string, CContestant<M_>> m_contestants;
	// todo

	/// @brief Finds a contestant or inserts a new contestant by their name
	typename map<string, CContestant<M_>>::iterator findContestant(const string &contestant) {
		auto iter = m_contestants.find(contestant);
		if (iter == m_contestants.end()) {
			iter = m_contestants.emplace(make_pair(contestant, CContestant<M_>(contestant))).first;
		}
		return iter;
	}

public:
	// default constructor
	// destructor
	// addMatch ( contestant1, contestant2, result )
	CContest &addMatch(const string &contestant1, const string &contestant2, const M_ &result) {
		if (contestant1 == contestant2) {
			throw logic_error("Attempted to add a match with one and the same contestant");
		}

		auto iter1 = findContestant(contestant1);

		if (iter1->second.contains(contestant2) || findContestant(contestant2)->second.contains(contestant1)) {
			throw logic_error("Match already added");
		}

		iter1->second.addMatch(contestant2, result);

		return *this;
	}
	// isOrdered ( comparator )
	// results ( comparator )
};

#ifndef __PROGTEST__
struct CMatch {
public:
	CMatch(int a,
		   int b)
		: m_A(a),
		  m_B(b) {
	}

	int m_A;
	int m_B;
};

class HigherScoreThreshold {
public:
	HigherScoreThreshold(int diffAtLeast)
		: m_DiffAtLeast(diffAtLeast) {
	}
	int operator()(const CMatch &x) const {
		return (x.m_A > x.m_B + m_DiffAtLeast) - (x.m_B > x.m_A + m_DiffAtLeast);
	}

private:
	int m_DiffAtLeast;
};

int HigherScore(const CMatch &x) {
	return (x.m_A > x.m_B) - (x.m_B > x.m_A);
}

int main(void) {
	CContest<CMatch> x;

	x.addMatch("C++", "Pascal", CMatch(10, 3))
		.addMatch("C++", "Java", CMatch(8, 1))
		.addMatch("Pascal", "Basic", CMatch(40, 0))
		.addMatch("Java", "PHP", CMatch(6, 2))
		.addMatch("Java", "Pascal", CMatch(7, 3))
		.addMatch("PHP", "Basic", CMatch(10, 0));

	assert(!x.isOrdered(HigherScore));
	try {
		list<string> res = x.results(HigherScore);
		assert("Exception missing!" == nullptr);
	} catch (const logic_error &e) {
	} catch (...) {
		assert("Invalid exception thrown!" == nullptr);
	}

	x.addMatch("PHP", "Pascal", CMatch(3, 6));

	assert(x.isOrdered(HigherScore));
	try {
		list<string> res = x.results(HigherScore);
		assert((res == list<string>{"C++", "Java", "Pascal", "PHP", "Basic"}));
	} catch (...) {
		assert("Unexpected exception!" == nullptr);
	}

	assert(!x.isOrdered(HigherScoreThreshold(3)));
	try {
		list<string> res = x.results(HigherScoreThreshold(3));
		assert("Exception missing!" == nullptr);
	} catch (const logic_error &e) {
	} catch (...) {
		assert("Invalid exception thrown!" == nullptr);
	}

	assert(x.isOrdered([](const CMatch &x) {
		return (x.m_A < x.m_B) - (x.m_B < x.m_A);
	}));
	try {
		list<string> res = x.results([](const CMatch &x) {
			return (x.m_A < x.m_B) - (x.m_B < x.m_A);
		});
		assert((res == list<string>{"Basic", "PHP", "Pascal", "Java", "C++"}));
	} catch (...) {
		assert("Unexpected exception!" == nullptr);
	}

	CContest<bool> y;

	y.addMatch("Python", "PHP", true)
		.addMatch("PHP", "Perl", true)
		.addMatch("Perl", "Bash", true)
		.addMatch("Bash", "JavaScript", true)
		.addMatch("JavaScript", "VBScript", true);

	assert(y.isOrdered([](bool v) {
		return v ? 10 : -10;
	}));
	try {
		list<string> res = y.results([](bool v) {
			return v ? 10 : -10;
		});
		assert((res == list<string>{"Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript"}));
	} catch (...) {
		assert("Unexpected exception!" == nullptr);
	}

	y.addMatch("PHP", "JavaScript", false);
	assert(!y.isOrdered([](bool v) {
		return v ? 10 : -10;
	}));
	try {
		list<string> res = y.results([](bool v) {
			return v ? 10 : -10;
		});
		assert("Exception missing!" == nullptr);
	} catch (const logic_error &e) {
	} catch (...) {
		assert("Invalid exception thrown!" == nullptr);
	}

	try {
		y.addMatch("PHP", "JavaScript", false);
		assert("Exception missing!" == nullptr);
	} catch (const logic_error &e) {
	} catch (...) {
		assert("Invalid exception thrown!" == nullptr);
	}

	try {
		y.addMatch("JavaScript", "PHP", true);
		assert("Exception missing!" == nullptr);
	} catch (const logic_error &e) {
	} catch (...) {
		assert("Invalid exception thrown!" == nullptr);
	}
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */