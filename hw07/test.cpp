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

class CSorter {
private:
	map<string, set<string>> m_family; // first is parent, second are their children
	bool m_isValid;

	/// @brief Finds a contestant or inserts a new contestant by their name
	map<string, set<string>>::iterator findContestant(const string &contestant) {
		auto iter = m_family.find(contestant);
		if (iter == m_family.end()) {
			iter = m_family.emplace(make_pair(contestant, set<string>())).first;
		}
		return iter;
	}

	// if test fails, first rework this to be more efficient
	bool expand() {
		bool expanded = false;
		// for every parent
		for (pair<string, set<string>> parent : m_family) {
			// for every of their child
			for (string child : parent.second) {
				// for the children of the child
				auto mapIter = m_family.find(child);
				for (auto iter = mapIter->second.begin(); iter != mapIter->second.end(); ++iter) {
					// attemp to add the children to the parent
					if (parent.first == (*iter)) {
						// loop found
						m_isValid = false;
						return false;
					}
					if (m_family[parent.first].insert(*iter).second) {
						expanded = true;
					}
				}
			}
		}
		return expanded;
	}

	void validate() {
		// finds for every pair if one can access the other and not the other way
		for (auto iterA = m_family.begin(); iterA != m_family.end(); ++iterA) {
			for (auto iterB = next(iterA); iterB != m_family.end(); ++iterB) {
				if (iterA->second.count(iterB->first) == iterB->second.count(iterA->first)) {
					m_isValid = false;
					return;
				}
			}
		}
	}

public:
	template <typename M_, typename F>
	CSorter(const map<string, CContestant<M_>> &contestants, F comparator) {
		m_isValid = true;
		// add the family and their children
		// if there is ever a result of zero, set validity to false and end
		for (auto iterContest = contestants.begin(); iterContest != contestants.end(); ++iterContest) {
			auto iterNode1 = findContestant(iterContest->first);
			for (auto pairChild : iterContest->second) {
				auto iterNode2 = findContestant(pairChild.first);
				int result = comparator(pairChild.second);
				if (result == 0) {
					m_isValid = false;
					return; // since we already know it's not valid
				}
				if (result > 0) {
					iterNode1->second.insert(iterNode2->first);
				}
				if (result < 0) {
					iterNode2->second.insert(iterNode1->first);
				}
			}
		}

		// everything added, now expand!
		// we will find loops this way too
		while (expand()) {
		}

		if (!m_isValid) { // if it's not valid, there's no point in validating
			return;
		}

		validate();
	}

	list<string> getList() const {
		vector<string> toReturn;
		for (auto pair : m_family) {
			toReturn.push_back(pair.first);
		}

		sort(toReturn.begin(), toReturn.end(), [this](const string &a, const string &b) { return m_family.find(a)->second.size() > m_family.find(b)->second.size(); });
		return list<string>(toReturn.begin(), toReturn.end());
	}

	bool getValid() const {
		return m_isValid;
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
	template <typename F>
	bool isOrdered(F comparator) {
		CSorter sorter(m_contestants, comparator);
		return sorter.getValid();
	}
	// results ( comparator )
	template <typename F>
	list<string> results(F comparator) {
		CSorter sorter(m_contestants, comparator);
		if (!sorter.getValid()) {
			throw logic_error("Attempted to sort something unsortable");
		}
		return sorter.getList();
	}
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