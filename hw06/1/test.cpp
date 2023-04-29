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
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

class CRect {
public:
	CRect(double x,
		  double y,
		  double w,
		  double h)
		: m_X(x),
		  m_Y(y),
		  m_W(w),
		  m_H(h) {
	}
	friend ostream &operator<<(ostream &os,
							   const CRect &x) {
		return os << '(' << x.m_X << ',' << x.m_Y << ',' << x.m_W << ',' << x.m_H << ')';
	}
	double m_X;
	double m_Y;
	double m_W;
	double m_H;
};
#endif /* __PROGTEST__ */

/// @brief Returns a new CRect with calculated absolute position
/// @param winPos The position of a window
/// @param relPos The relative position of an element
/// @return The absolute position of an element
CRect getAbsolutePos(const CRect &winPos, const CRect &relPos) {
	return CRect(
		(winPos.m_W * relPos.m_X) + winPos.m_X,
		(winPos.m_H * relPos.m_Y) + winPos.m_Y,
		winPos.m_W * relPos.m_W,
		winPos.m_H * relPos.m_H);
}

/// @brief Checks if a sorted vector of ints contains a given value
/// @param checking Value to find
/// @param toCheck Vector to check
/// @return True if value was found, otherwise false
bool isContained(const int &checking, const vector<int> &toCheck) {
	auto it = lower_bound(toCheck.begin(), toCheck.end(), checking);
	return it != toCheck.end();
}

class CElement {
private:
	int m_id;

protected:
	CRect m_relPos;
	CRect m_absPos;

	// functions to override
	void printIndent(ostream &os, const int &indent, vector<int> &pipePos, const bool &printLastSpace = true) const {
		for (int i = 0; i < indent; i++) {
			if (i == indent - 1) {
				os << ((printLastSpace) ? ("+- ") : ("+-"));
				return;
			}
			if (isContained(i, pipePos)) {
				os << "|  ";
				continue;
			}
			os << "   ";
		}
	}
	void printCommon(ostream &os, const int &indent, vector<int> &pipePos) const {
		printIndent(os, indent, pipePos);
		os << "[" << m_id << "] " << getElementName() << " " << m_absPos << "\n";
	}
	void printCommon(ostream &os, const int &indent, vector<int> &pipePos, const string &title) const {
		printIndent(os, indent, pipePos);
		os << "[" << m_id << "] " << getElementName() << " \"" << title << "\" " << m_absPos << "\n";
	}
	virtual string getElementName() const = 0;

public:
	CElement(const int &id, const CRect &relPos) : m_id(id), m_relPos(relPos), m_absPos(CRect(0, 0, 0, 0)) {}

	virtual shared_ptr<CElement> clone() const = 0;

	virtual void print(ostream &os, int indent, vector<int> &pipePos) const = 0;

	int getId() {
		return m_id;
	}
};

class CTitleable : public CElement {
private:
protected:
	string m_title;

public:
	CTitleable(const int &id, const CRect &relPos, const string &title) : CElement(id, relPos), m_title(title) {}

	virtual void print(ostream &os, int indent, vector<int> &pipePos) const override {
		printCommon(os, indent, pipePos, m_title);
	}
};

// I'm not inheriting from CTitleable for I will later copy this implementation into hw06_2 that will inherit from CPanel which will not inherit from CTitleable
class CWindow : public CElement {
private:
	vector<shared_ptr<CElement>> m_addOrder;
	multimap<int, weak_ptr<CElement>> m_elements;
	string m_title;

	virtual string getElementName() const override {
		return "Window";
	}

public:
	CWindow(const int &id, const string &title, const CRect &absPos) : CElement(id, absPos), m_title(title) {
		swap(m_absPos, m_relPos);
	}

	virtual shared_ptr<CElement> clone() const override {
		return make_shared<CWindow>(*this);
	}

	virtual void print(ostream &os, int indent, vector<int> &pipePos) const override {
		printCommon(os, indent, pipePos, m_title);
		pipePos.push_back(indent++);

		for (auto iter = m_addOrder.begin(); iter != m_addOrder.end(); ++iter) {
			if (next(iter) == m_addOrder.end()) { // Element is second last
				pipePos.pop_back();
			}
			(*iter)->print(os, indent, pipePos);
		}
	}

	// add
	CWindow &add(const CElement &toAdd) {
		shared_ptr<CElement> addSptr = toAdd.clone();
		weak_ptr<CElement> addWptr = addSptr;

		m_addOrder.push_back(addSptr);
		m_elements.insert(make_pair(addSptr->getId(), addWptr));
		return *this;
	}

	// search
	// setPosition
};

class CButton : public CTitleable {
private:
	virtual string getElementName() const override {
		return "Button";
	}

public:
	using CTitleable::CTitleable;

	virtual shared_ptr<CElement> clone() const override {
		return make_shared<CButton>(*this);
	}
};

class CInput : public CTitleable {
private:
	virtual string getElementName() const override {
		return "Input";
	}

public:
	using CTitleable::CTitleable;

	virtual shared_ptr<CElement> clone() const override {
		return make_shared<CInput>(*this);
	}

	// setValue
	// getValue
};
class CLabel : public CTitleable {
private:
	virtual string getElementName() const override {
		return "Label";
	}

public:
	using CTitleable::CTitleable;

	virtual shared_ptr<CElement> clone() const override {
		return make_shared<CLabel>(*this);
	}
};

class CComboBox : public CElement {
private:
	vector<string> m_items;
	size_t m_selected;

	virtual string getElementName() const override {
		return "ComboBox";
	}

public:
	using CElement::CElement;

	virtual void print(ostream &os, int indent, vector<int> &pipePos) const override {
		printCommon(os, indent, pipePos);
		indent++;
		for (size_t i = 0; i < m_items.size(); ++i) {
			printIndent(os, indent, pipePos, false);
			os << ((i == m_selected) ? (">" + m_items[i] + "<") : (" " + m_items[i]));
			os << "\n";
		}
	}

	virtual shared_ptr<CElement> clone() const override {
		return make_shared<CComboBox>(*this);
	}
	// add
	CComboBox &add(const string &toAdd) {
		m_items.emplace_back(string(toAdd));
		return *this;
	}
	// setSelected
	// getSelected
};

// output operators
ostream &operator<<(ostream &os, const CElement &element) {
	vector<int> pipePos;
	element.print(os, 0, pipePos);
	return os;
}

#ifndef __PROGTEST__
template <typename _T>
string toString(const _T &x) {
	ostringstream oss;
	oss << x;
	return oss.str();
}

int main(void) {
	assert(sizeof(CButton) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
	assert(sizeof(CInput) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
	assert(sizeof(CLabel) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
	CWindow a(0, "Sample window", CRect(10, 10, 600, 480));
	a.add(CButton(1, CRect(0.1, 0.8, 0.3, 0.1), "Ok")).add(CButton(2, CRect(0.6, 0.8, 0.3, 0.1), "Cancel"));
	a.add(CLabel(10, CRect(0.1, 0.1, 0.2, 0.1), "Username:"));
	a.add(CInput(11, CRect(0.4, 0.1, 0.5, 0.1), "chucknorris"));
	a.add(CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest"));
	assert(toString(a) ==
		   "[0] Window \"Sample window\" (10,10,600,480)\n"
		   "+- [1] Button \"Ok\" (70,394,180,48)\n"
		   "+- [2] Button \"Cancel\" (370,394,180,48)\n"
		   "+- [10] Label \"Username:\" (70,58,120,48)\n"
		   "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
		   "+- [20] ComboBox (70,154,480,48)\n"
		   "   +->Karate<\n"
		   "   +- Judo\n"
		   "   +- Box\n"
		   "   +- Progtest\n");
	CWindow b = a;
	assert(toString(*b.search(20)) ==
		   "[20] ComboBox (70,154,480,48)\n"
		   "+->Karate<\n"
		   "+- Judo\n"
		   "+- Box\n"
		   "+- Progtest\n");
	assert(dynamic_cast<CComboBox &>(*b.search(20)).getSelected() == 0);
	dynamic_cast<CComboBox &>(*b.search(20)).setSelected(3);
	assert(dynamic_cast<CInput &>(*b.search(11)).getValue() == "chucknorris");
	dynamic_cast<CInput &>(*b.search(11)).setValue("chucknorris@fit.cvut.cz");
	b.add(CComboBox(21, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
	assert(toString(b) ==
		   "[0] Window \"Sample window\" (10,10,600,480)\n"
		   "+- [1] Button \"Ok\" (70,394,180,48)\n"
		   "+- [2] Button \"Cancel\" (370,394,180,48)\n"
		   "+- [10] Label \"Username:\" (70,58,120,48)\n"
		   "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
		   "+- [20] ComboBox (70,154,480,48)\n"
		   "|  +- Karate\n"
		   "|  +- Judo\n"
		   "|  +- Box\n"
		   "|  +->Progtest<\n"
		   "+- [21] ComboBox (70,250,480,48)\n"
		   "   +->PA2<\n"
		   "   +- OSY\n"
		   "   +- Both\n");
	assert(toString(a) ==
		   "[0] Window \"Sample window\" (10,10,600,480)\n"
		   "+- [1] Button \"Ok\" (70,394,180,48)\n"
		   "+- [2] Button \"Cancel\" (370,394,180,48)\n"
		   "+- [10] Label \"Username:\" (70,58,120,48)\n"
		   "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
		   "+- [20] ComboBox (70,154,480,48)\n"
		   "   +->Karate<\n"
		   "   +- Judo\n"
		   "   +- Box\n"
		   "   +- Progtest\n");
	b.setPosition(CRect(20, 30, 640, 520));
	assert(toString(b) ==
		   "[0] Window \"Sample window\" (20,30,640,520)\n"
		   "+- [1] Button \"Ok\" (84,446,192,52)\n"
		   "+- [2] Button \"Cancel\" (404,446,192,52)\n"
		   "+- [10] Label \"Username:\" (84,82,128,52)\n"
		   "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
		   "+- [20] ComboBox (84,186,512,52)\n"
		   "|  +- Karate\n"
		   "|  +- Judo\n"
		   "|  +- Box\n"
		   "|  +->Progtest<\n"
		   "+- [21] ComboBox (84,290,512,52)\n"
		   "   +->PA2<\n"
		   "   +- OSY\n"
		   "   +- Both\n");
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */