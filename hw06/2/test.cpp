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

	// common methods
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
	// methods to override

	virtual bool isCWindow() const {
		return false;
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

	int getId() const {
		return m_id;
	}

	virtual void updatePosition(const CRect &parentAbsPos) {
		if (!isCWindow()) {
			m_absPos = getAbsolutePos(parentAbsPos, m_relPos);
		}
	}

	virtual CElement *search(const int &id) {
		return (m_id == id) ? this : nullptr;
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

class CPanel {
public:
	CPanel(int id,
		   const CRect &relPos);
	// add
};

// I'm not inheriting from CTitleable for I will later copy this implementation into hw06_2 that will inherit from CPanel which will not inherit from CTitleable
class CWindow : public CElement {
private:
	vector<shared_ptr<CElement>> m_elements;
	string m_title;

	virtual string getElementName() const override {
		return "Window";
	}

	virtual bool isCWindow() const override {
		return true;
	}

protected:
	virtual void updateChildrenPos() {
		for (auto element : m_elements) {
			element->updatePosition(m_absPos);
		}
	}

public:
	CWindow(const int &id, const string &title, const CRect &absPos) : CElement(id, absPos), m_title(title) {
		swap(m_absPos, m_relPos);
	}

	CWindow(const CWindow &copyFrom) : CWindow(copyFrom.getId(), copyFrom.m_title, copyFrom.m_absPos) {
		m_elements.clear();

		for (auto element : copyFrom.m_elements) {
			add(*element);
		}
	}

	virtual shared_ptr<CElement> clone() const override {
		return make_shared<CWindow>(*this);
	}

	virtual void print(ostream &os, int indent, vector<int> &pipePos) const override {
		printCommon(os, indent, pipePos, m_title);
		if (m_elements.size() > 1) {
			pipePos.push_back(indent);
		}
		++indent;

		for (auto iter = m_elements.begin(); iter != m_elements.end(); ++iter) {
			if (next(iter) == m_elements.end() && m_elements.size() > 1) { // Element is second last
				pipePos.pop_back();
			}
			(*iter)->print(os, indent, pipePos);
		}
	}

	// add
	CWindow &add(const CElement &toAdd) {
		shared_ptr<CElement> addSptr = toAdd.clone();
		weak_ptr<CElement> addWptr = addSptr;

		// update the position of the child
		addSptr->updatePosition(m_absPos);

		m_elements.push_back(addSptr);
		return *this;
	}

	// search
	virtual CElement *search(const int &id) override {
		if (id == getId()) {
			return this;
		}
		for (auto iter = m_elements.begin(); iter != m_elements.end(); ++iter) {
			CElement *res = (*iter)->search(id);
			if (res != nullptr) {
				return res;
			}
		}

		return nullptr;
	}
	// setPosition
	void setPosition(const CRect &newPos) {
		m_absPos = newPos;
		updateChildrenPos();
	}
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
	void setValue(const string &newVal) {
		m_title = newVal;
	}
	// getValue
	string getValue() const {
		return m_title;
	}
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
	CComboBox(const int &id, const CRect &relPos) : CElement(id, relPos) {
		m_selected = 0;
	}

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
	void setSelected(const size_t &selected) {
		m_selected = selected;
	}
	// getSelected
	size_t getSelected() const {
		return m_selected;
	}
};

// output operators
ostream &operator<<(ostream &os, const CElement &element) {
	vector<int> pipePos;
	element.print(os, 0, pipePos);
	return os;
}

#ifndef __PROGTEST__
template <typename T_>
string toString(const T_ &x) {
	ostringstream oss;
	oss << x;
	return oss.str();
}

void testWindowInsideWindow1() {
	CWindow a(0, "a", CRect(10, 10, 600, 480));
	CWindow b(10, "b", CRect(20, 30, 640, 520));
	a.add(CComboBox(1, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest"));
	a.add(b);
	a.add(CComboBox(2, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
	assert(toString(a) ==
		   "[0] Window \"a\" (10,10,600,480)\n"
		   "+- [1] ComboBox (70,154,480,48)\n"
		   "|  +->Karate<\n"
		   "|  +- Judo\n"
		   "|  +- Box\n"
		   "|  +- Progtest\n"
		   "+- [10] Window \"b\" (20,30,640,520)\n"
		   "+- [2] ComboBox (70,250,480,48)\n"
		   "   +->PA2<\n"
		   "   +- OSY\n"
		   "   +- Both\n");
	dynamic_cast<CWindow &>(*a.search(10))
		.add(CComboBox(11, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest"))
		.add(CComboBox(12, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
	assert(toString(b) ==
		   "[10] Window \"b\" (20,30,640,520)\n");
	assert(toString(*a.search(10)) ==
		   "[10] Window \"b\" (20,30,640,520)\n"
		   "+- [11] ComboBox (84,186,512,52)\n"
		   "|  +->Karate<\n"
		   "|  +- Judo\n"
		   "|  +- Box\n"
		   "|  +- Progtest\n"
		   "+- [12] ComboBox (84,290,512,52)\n"
		   "   +->PA2<\n"
		   "   +- OSY\n"
		   "   +- Both\n");
	assert(toString(a) ==
		   "[0] Window \"a\" (10,10,600,480)\n"
		   "+- [1] ComboBox (70,154,480,48)\n"
		   "|  +->Karate<\n"
		   "|  +- Judo\n"
		   "|  +- Box\n"
		   "|  +- Progtest\n"
		   "+- [10] Window \"b\" (20,30,640,520)\n"
		   "|  +- [11] ComboBox (84,186,512,52)\n"
		   "|  |  +->Karate<\n"
		   "|  |  +- Judo\n"
		   "|  |  +- Box\n"
		   "|  |  +- Progtest\n"
		   "|  +- [12] ComboBox (84,290,512,52)\n"
		   "|     +->PA2<\n"
		   "|     +- OSY\n"
		   "|     +- Both\n"
		   "+- [2] ComboBox (70,250,480,48)\n"
		   "   +->PA2<\n"
		   "   +- OSY\n"
		   "   +- Both\n");
	a.setPosition(CRect(0, 0, 1000, 1000));
	assert(toString(a) ==
		   "[0] Window \"a\" (0,0,1000,1000)\n"
		   "+- [1] ComboBox (100,300,800,100)\n"
		   "|  +->Karate<\n"
		   "|  +- Judo\n"
		   "|  +- Box\n"
		   "|  +- Progtest\n"
		   "+- [10] Window \"b\" (20,30,640,520)\n"
		   "|  +- [11] ComboBox (84,186,512,52)\n"
		   "|  |  +->Karate<\n"
		   "|  |  +- Judo\n"
		   "|  |  +- Box\n"
		   "|  |  +- Progtest\n"
		   "|  +- [12] ComboBox (84,290,512,52)\n"
		   "|     +->PA2<\n"
		   "|     +- OSY\n"
		   "|     +- Both\n"
		   "+- [2] ComboBox (100,500,800,100)\n"
		   "   +->PA2<\n"
		   "   +- OSY\n"
		   "   +- Both\n");
}

void testWindowInsideWindow2() {
	CWindow a(1, "a", CRect(0, 0, 100, 100));
	CWindow b(2, "b", CRect(10, 10, 200, 200));
	a.add(CButton(3, CRect(0.5, 0.5, 0.1, 0.1), "OK"));
	b.add(a);
	cout << b;
	assert(toString(b) ==
		   "[2] Window \"b\" (10,10,200,200)\n"
		   "+- [1] Window \"a\" (0,0,100,100)\n"
		   "   +- [3] Button \"OK\" (50,50,10,10)\n");
}

void testPart1() {
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

	testWindowInsideWindow1();
	testWindowInsideWindow2();
}

int main(void) {
	testPart1();
	assert(sizeof(CButton) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
	assert(sizeof(CInput) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
	assert(sizeof(CLabel) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
	assert(sizeof(CButton) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
	assert(sizeof(CInput) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
	assert(sizeof(CLabel) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
	CWindow a(0, "Sample window", CRect(10, 10, 600, 480));
	a.add(CButton(1, CRect(0.1, 0.8, 0.3, 0.1), "Ok")).add(CButton(2, CRect(0.6, 0.8, 0.3, 0.1), "Cancel"));
	a.add(CLabel(10, CRect(0.1, 0.1, 0.2, 0.1), "Username:"));
	a.add(CInput(11, CRect(0.4, 0.1, 0.5, 0.1), "chucknorris"));
	a.add(CPanel(12, CRect(0.1, 0.3, 0.8, 0.7)).add(CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest")));
	assert(toString(a) ==
		   "[0] Window \"Sample window\" (10,10,600,480)\n"
		   "+- [1] Button \"Ok\" (70,394,180,48)\n"
		   "+- [2] Button \"Cancel\" (370,394,180,48)\n"
		   "+- [10] Label \"Username:\" (70,58,120,48)\n"
		   "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
		   "+- [12] Panel (70,154,480,336)\n"
		   "   +- [20] ComboBox (118,254.8,384,33.6)\n"
		   "      +->Karate<\n"
		   "      +- Judo\n"
		   "      +- Box\n"
		   "      +- Progtest\n");
	CWindow b = a;
	assert(toString(*b.search(20)) ==
		   "[20] ComboBox (118,254.8,384,33.6)\n"
		   "+->Karate<\n"
		   "+- Judo\n"
		   "+- Box\n"
		   "+- Progtest\n");
	assert(dynamic_cast<CComboBox &>(*b.search(20)).getSelected() == 0);
	dynamic_cast<CComboBox &>(*b.search(20)).setSelected(3);
	assert(dynamic_cast<CInput &>(*b.search(11)).getValue() == "chucknorris");
	dynamic_cast<CInput &>(*b.search(11)).setValue("chucknorris@fit.cvut.cz");
	CPanel &p = dynamic_cast<CPanel &>(*b.search(12));
	p.add(CComboBox(21, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
	assert(toString(b) ==
		   "[0] Window \"Sample window\" (10,10,600,480)\n"
		   "+- [1] Button \"Ok\" (70,394,180,48)\n"
		   "+- [2] Button \"Cancel\" (370,394,180,48)\n"
		   "+- [10] Label \"Username:\" (70,58,120,48)\n"
		   "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
		   "+- [12] Panel (70,154,480,336)\n"
		   "   +- [20] ComboBox (118,254.8,384,33.6)\n"
		   "   |  +- Karate\n"
		   "   |  +- Judo\n"
		   "   |  +- Box\n"
		   "   |  +->Progtest<\n"
		   "   +- [21] ComboBox (118,322,384,33.6)\n"
		   "      +->PA2<\n"
		   "      +- OSY\n"
		   "      +- Both\n");
	assert(toString(a) ==
		   "[0] Window \"Sample window\" (10,10,600,480)\n"
		   "+- [1] Button \"Ok\" (70,394,180,48)\n"
		   "+- [2] Button \"Cancel\" (370,394,180,48)\n"
		   "+- [10] Label \"Username:\" (70,58,120,48)\n"
		   "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
		   "+- [12] Panel (70,154,480,336)\n"
		   "   +- [20] ComboBox (118,254.8,384,33.6)\n"
		   "      +->Karate<\n"
		   "      +- Judo\n"
		   "      +- Box\n"
		   "      +- Progtest\n");
	assert(toString(p) ==
		   "[12] Panel (70,154,480,336)\n"
		   "+- [20] ComboBox (118,254.8,384,33.6)\n"
		   "|  +- Karate\n"
		   "|  +- Judo\n"
		   "|  +- Box\n"
		   "|  +->Progtest<\n"
		   "+- [21] ComboBox (118,322,384,33.6)\n"
		   "   +->PA2<\n"
		   "   +- OSY\n"
		   "   +- Both\n");
	b.setPosition(CRect(20, 30, 640, 520));
	assert(toString(b) ==
		   "[0] Window \"Sample window\" (20,30,640,520)\n"
		   "+- [1] Button \"Ok\" (84,446,192,52)\n"
		   "+- [2] Button \"Cancel\" (404,446,192,52)\n"
		   "+- [10] Label \"Username:\" (84,82,128,52)\n"
		   "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
		   "+- [12] Panel (84,186,512,364)\n"
		   "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
		   "   |  +- Karate\n"
		   "   |  +- Judo\n"
		   "   |  +- Box\n"
		   "   |  +->Progtest<\n"
		   "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
		   "      +->PA2<\n"
		   "      +- OSY\n"
		   "      +- Both\n");
	p.add(p);
	assert(toString(p) ==
		   "[12] Panel (84,186,512,364)\n"
		   "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
		   "|  +- Karate\n"
		   "|  +- Judo\n"
		   "|  +- Box\n"
		   "|  +->Progtest<\n"
		   "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
		   "|  +->PA2<\n"
		   "|  +- OSY\n"
		   "|  +- Both\n"
		   "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
		   "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
		   "   |  +- Karate\n"
		   "   |  +- Judo\n"
		   "   |  +- Box\n"
		   "   |  +->Progtest<\n"
		   "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
		   "      +->PA2<\n"
		   "      +- OSY\n"
		   "      +- Both\n");
	p.add(p);
	assert(toString(p) ==
		   "[12] Panel (84,186,512,364)\n"
		   "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
		   "|  +- Karate\n"
		   "|  +- Judo\n"
		   "|  +- Box\n"
		   "|  +->Progtest<\n"
		   "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
		   "|  +->PA2<\n"
		   "|  +- OSY\n"
		   "|  +- Both\n"
		   "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
		   "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
		   "|  |  +- Karate\n"
		   "|  |  +- Judo\n"
		   "|  |  +- Box\n"
		   "|  |  +->Progtest<\n"
		   "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
		   "|     +->PA2<\n"
		   "|     +- OSY\n"
		   "|     +- Both\n"
		   "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
		   "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
		   "   |  +- Karate\n"
		   "   |  +- Judo\n"
		   "   |  +- Box\n"
		   "   |  +->Progtest<\n"
		   "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
		   "   |  +->PA2<\n"
		   "   |  +- OSY\n"
		   "   |  +- Both\n"
		   "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
		   "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
		   "      |  +- Karate\n"
		   "      |  +- Judo\n"
		   "      |  +- Box\n"
		   "      |  +->Progtest<\n"
		   "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
		   "         +->PA2<\n"
		   "         +- OSY\n"
		   "         +- Both\n");
	p.add(p);
	assert(toString(p) ==
		   "[12] Panel (84,186,512,364)\n"
		   "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
		   "|  +- Karate\n"
		   "|  +- Judo\n"
		   "|  +- Box\n"
		   "|  +->Progtest<\n"
		   "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
		   "|  +->PA2<\n"
		   "|  +- OSY\n"
		   "|  +- Both\n"
		   "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
		   "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
		   "|  |  +- Karate\n"
		   "|  |  +- Judo\n"
		   "|  |  +- Box\n"
		   "|  |  +->Progtest<\n"
		   "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
		   "|     +->PA2<\n"
		   "|     +- OSY\n"
		   "|     +- Both\n"
		   "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
		   "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
		   "|  |  +- Karate\n"
		   "|  |  +- Judo\n"
		   "|  |  +- Box\n"
		   "|  |  +->Progtest<\n"
		   "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
		   "|  |  +->PA2<\n"
		   "|  |  +- OSY\n"
		   "|  |  +- Both\n"
		   "|  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
		   "|     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
		   "|     |  +- Karate\n"
		   "|     |  +- Judo\n"
		   "|     |  +- Box\n"
		   "|     |  +->Progtest<\n"
		   "|     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
		   "|        +->PA2<\n"
		   "|        +- OSY\n"
		   "|        +- Both\n"
		   "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
		   "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
		   "   |  +- Karate\n"
		   "   |  +- Judo\n"
		   "   |  +- Box\n"
		   "   |  +->Progtest<\n"
		   "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
		   "   |  +->PA2<\n"
		   "   |  +- OSY\n"
		   "   |  +- Both\n"
		   "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
		   "   |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
		   "   |  |  +- Karate\n"
		   "   |  |  +- Judo\n"
		   "   |  |  +- Box\n"
		   "   |  |  +->Progtest<\n"
		   "   |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
		   "   |     +->PA2<\n"
		   "   |     +- OSY\n"
		   "   |     +- Both\n"
		   "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
		   "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
		   "      |  +- Karate\n"
		   "      |  +- Judo\n"
		   "      |  +- Box\n"
		   "      |  +->Progtest<\n"
		   "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
		   "      |  +->PA2<\n"
		   "      |  +- OSY\n"
		   "      |  +- Both\n"
		   "      +- [12] Panel (208.928,425.148,262.144,124.852)\n"
		   "         +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
		   "         |  +- Karate\n"
		   "         |  +- Judo\n"
		   "         |  +- Box\n"
		   "         |  +->Progtest<\n"
		   "         +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
		   "            +->PA2<\n"
		   "            +- OSY\n"
		   "            +- Both\n");
	assert(toString(b) ==
		   "[0] Window \"Sample window\" (20,30,640,520)\n"
		   "+- [1] Button \"Ok\" (84,446,192,52)\n"
		   "+- [2] Button \"Cancel\" (404,446,192,52)\n"
		   "+- [10] Label \"Username:\" (84,82,128,52)\n"
		   "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
		   "+- [12] Panel (84,186,512,364)\n"
		   "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
		   "   |  +- Karate\n"
		   "   |  +- Judo\n"
		   "   |  +- Box\n"
		   "   |  +->Progtest<\n"
		   "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
		   "   |  +->PA2<\n"
		   "   |  +- OSY\n"
		   "   |  +- Both\n"
		   "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
		   "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
		   "   |  |  +- Karate\n"
		   "   |  |  +- Judo\n"
		   "   |  |  +- Box\n"
		   "   |  |  +->Progtest<\n"
		   "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
		   "   |     +->PA2<\n"
		   "   |     +- OSY\n"
		   "   |     +- Both\n"
		   "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
		   "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
		   "   |  |  +- Karate\n"
		   "   |  |  +- Judo\n"
		   "   |  |  +- Box\n"
		   "   |  |  +->Progtest<\n"
		   "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
		   "   |  |  +->PA2<\n"
		   "   |  |  +- OSY\n"
		   "   |  |  +- Both\n"
		   "   |  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
		   "   |     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
		   "   |     |  +- Karate\n"
		   "   |     |  +- Judo\n"
		   "   |     |  +- Box\n"
		   "   |     |  +->Progtest<\n"
		   "   |     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
		   "   |        +->PA2<\n"
		   "   |        +- OSY\n"
		   "   |        +- Both\n"
		   "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
		   "      +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
		   "      |  +- Karate\n"
		   "      |  +- Judo\n"
		   "      |  +- Box\n"
		   "      |  +->Progtest<\n"
		   "      +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
		   "      |  +->PA2<\n"
		   "      |  +- OSY\n"
		   "      |  +- Both\n"
		   "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
		   "      |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
		   "      |  |  +- Karate\n"
		   "      |  |  +- Judo\n"
		   "      |  |  +- Box\n"
		   "      |  |  +->Progtest<\n"
		   "      |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
		   "      |     +->PA2<\n"
		   "      |     +- OSY\n"
		   "      |     +- Both\n"
		   "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
		   "         +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
		   "         |  +- Karate\n"
		   "         |  +- Judo\n"
		   "         |  +- Box\n"
		   "         |  +->Progtest<\n"
		   "         +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
		   "         |  +->PA2<\n"
		   "         |  +- OSY\n"
		   "         |  +- Both\n"
		   "         +- [12] Panel (208.928,425.148,262.144,124.852)\n"
		   "            +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
		   "            |  +- Karate\n"
		   "            |  +- Judo\n"
		   "            |  +- Box\n"
		   "            |  +->Progtest<\n"
		   "            +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
		   "               +->PA2<\n"
		   "               +- OSY\n"
		   "               +- Both\n");
	assert(toString(a) ==
		   "[0] Window \"Sample window\" (10,10,600,480)\n"
		   "+- [1] Button \"Ok\" (70,394,180,48)\n"
		   "+- [2] Button \"Cancel\" (370,394,180,48)\n"
		   "+- [10] Label \"Username:\" (70,58,120,48)\n"
		   "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
		   "+- [12] Panel (70,154,480,336)\n"
		   "   +- [20] ComboBox (118,254.8,384,33.6)\n"
		   "      +->Karate<\n"
		   "      +- Judo\n"
		   "      +- Box\n"
		   "      +- Progtest\n");
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */