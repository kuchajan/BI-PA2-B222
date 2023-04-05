#ifndef __PROGTEST__
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

struct MyString {
private:
	size_t m_allocSize;
	size_t m_used;
	void realloc(size_t newAllocSize);
	void addVal(const char &toAdd);

public:
	char *m_val;
	MyString(const char *value); // constructor
	MyString(const MyString &);	 // copy constructor
	~MyString();				 // destructor
	friend ostream &operator<<(ostream &os, const MyString &s);
	bool operator==(const MyString &other) const;
	bool operator<(const MyString &other) const;
	bool operator>(const MyString &other) const;
};

void MyString::realloc(size_t newAllocSize) {
	char *newVal = new char[newAllocSize];
	memcpy(newVal, m_val, min(m_allocSize, newAllocSize));
	delete[] m_val;
	m_val = newVal;
	m_allocSize = newAllocSize;
}

void MyString::addVal(const char &toAdd) {
	if (m_used + 1 > m_allocSize) {
		MyString::realloc(m_allocSize * 2);
	}
	m_val[m_used++] = toAdd;
}

MyString::MyString(const char *value) {
	m_allocSize = 16;
	m_used = 0;
	m_val = new char[m_allocSize];
	while (*value != '\0') {
		addVal(*value);
		value++;
	}
	addVal('\0');
	MyString::realloc(m_used);
}

MyString::MyString(const MyString &other) {
	m_allocSize = other.m_used;
	m_used = 0;
	m_val = new char[other.m_used];
	while (m_used != other.m_used) {
		addVal(other.m_val[m_used]);
	}
}

MyString::~MyString() {
	delete[] m_val;
}

ostream &operator<<(ostream &os, const MyString &s) {
	os << s.m_val;
	return os;
}

bool MyString::operator==(const MyString &other) const {
	if (m_used != other.m_used) {
		return false;
	}
	return strcmp(m_val, other.m_val) == 0;
}

bool MyString::operator<(const MyString &other) const {
	return strcmp(m_val, other.m_val) < 0;
}

bool MyString::operator>(const MyString &other) const {
	return strcmp(m_val, other.m_val) > 0;
}

template <class T>
class CShared_ptr {
private:
	T *m_val;
	size_t *m_occurences;

public:
	CShared_ptr(const T &);			// constructor
	CShared_ptr(CShared_ptr &); // copy-constructor
	~CShared_ptr();				// destructor
	const T &operator*() const; // getter
};

template <class T>
CShared_ptr<T>::CShared_ptr(const T &value) {
	m_occurences = new size_t(1);
	m_val = new T(value);
}

template <class T>
CShared_ptr<T>::CShared_ptr(CShared_ptr &copyFrom) : m_val(copyFrom.m_val), m_occurences(copyFrom.m_occurences) {
	(*m_occurences)++;
}

template <class T>
CShared_ptr<T>::~CShared_ptr() {
	if (--(*m_occurences) == 0) {
		delete m_val;
		delete m_occurences;
	}
}

template <class T>
const T &CShared_ptr<T>::operator*() const {
	return *m_val;
}

class CMailServer;

class CMail {
private:
	MyString m_From;
	MyString m_To;
	MyString m_Body;

	friend CMailServer;
public:
	CMail(const char *from,
		  const char *to,
		  const char *body);
	bool operator==(const CMail &x) const;
	friend ostream &operator<<(ostream &os,
							   const CMail &m);
};

CMail::CMail(const char *from, const char *to, const char *body)
	: m_From(from), m_To(to), m_Body(body) {}

bool CMail::operator==(const CMail &x) const {
	return (m_From == x.m_From) && (m_To == x.m_To) && (m_Body == x.m_Body);
}

ostream &operator<<(ostream &os, const CMail &m) {
	os << "From:" << m.m_From << ", To: " << m.m_To << ", Body: " << m.m_Body;
	return os;
}

class CLinkList {
private:
	struct SLLNode {
		CShared_ptr<CMail> m_val;
		SLLNode *m_next;
		SLLNode(CShared_ptr<CMail> &newVal);
	};
	SLLNode *m_head;
	SLLNode *m_tail;

public:
	CLinkList();				  // constructor
	CLinkList(const CLinkList &); // copy constructor
	~CLinkList();				  // destructor

	void pushback(CShared_ptr<CMail> &toPushBack);
	void popfront();
	const CShared_ptr<CMail> getfront() const;
	bool isHeadNull() const;
};

CLinkList::SLLNode::SLLNode(CShared_ptr<CMail> &newVal)
	: m_val(newVal) {
	m_next = nullptr;
}

CLinkList::CLinkList() {
	m_head = nullptr;
	m_tail = nullptr;
}

CLinkList::CLinkList(const CLinkList &copyFrom) {
	m_head = nullptr;
	m_tail = nullptr;
	SLLNode *current = copyFrom.m_head;
	while (current != nullptr) {
		pushback(current->m_val);
		current = current->m_next;
	}
}

CLinkList::~CLinkList() {
	while (m_head != nullptr) {
		popfront();
	}
}

void CLinkList::pushback(CShared_ptr<CMail> &toPushBack) {
	SLLNode *newNode = new SLLNode(toPushBack);
	if (m_head == nullptr) {
		m_head = m_tail = newNode;
		return;
	}

	m_tail->m_next = newNode;
	m_tail = newNode;
}

void CLinkList::popfront() {
	if (m_head != nullptr) {
		SLLNode *oldHead = m_head;
		m_head = m_head->m_next;
		delete oldHead;
	}
	if (m_head == nullptr) {
		m_tail = nullptr;
	}
}

const CShared_ptr<CMail> CLinkList::getfront() const {
	return m_head->m_val;
}

bool CLinkList::isHeadNull() const {
	return m_head == nullptr;
}

class CMailIterator {
private:
	CLinkList m_mailList;

public:
	CMailIterator();
	CMailIterator(const CLinkList &);

	explicit operator bool(void) const;
	bool operator!(void) const;
	const CMail &operator*(void) const;
	CMailIterator &operator++(void);
};

CMailIterator::CMailIterator() : m_mailList() {}

CMailIterator::CMailIterator(const CLinkList &mailList) : m_mailList(mailList) {}

CMailIterator::operator bool() const {
	return !m_mailList.isHeadNull();
}

bool CMailIterator::operator!() const {
	return m_mailList.isHeadNull();
}

const CMail &CMailIterator::operator*() const {
	return m_mailList.getfront().operator*();
}

CMailIterator &CMailIterator::operator++() {
	m_mailList.popfront();
}

class CPerson {
private:
	MyString m_address;

	CLinkList m_inbox;
	CLinkList m_outbox;

public:
	CPerson(const MyString &address);

	CMailIterator inbox();
	CMailIterator outbox();

	void receive(CShared_ptr<CMail> &mail);
	void send(CShared_ptr<CMail> &mail);

	bool operator==(const CPerson &other) const;
	bool operator!=(const CPerson &other) const;
	bool operator<(const CPerson &other) const;
	bool operator>(const CPerson &other) const;
};

bool CPerson::operator==(const CPerson & other) const {
	return m_address == other.m_address;
}

bool CPerson::operator!=(const CPerson &other) const {
	return !(m_address == other.m_address);
}

bool CPerson::operator<(const CPerson & other) const {
	return m_address < other.m_address;
}

bool CPerson::operator>(const CPerson & other) const {
	return m_address > other.m_address;
}

CPerson::CPerson(const MyString &address) : m_address(address), m_inbox(), m_outbox() {}

CMailIterator CPerson::inbox() {
	return CMailIterator(m_inbox);
}

CMailIterator CPerson::outbox() {
	return CMailIterator(m_outbox);
}

void CPerson::receive(CShared_ptr<CMail> &mail) {
	m_inbox.pushback(mail);
}

void CPerson::send(CShared_ptr<CMail> &mail) {
	m_outbox.pushback(mail);
}

class CMailServer {
private:
	// todo
public:
	CMailServer(void);
	CMailServer(const CMailServer &src);
	CMailServer &operator=(const CMailServer &src);
	~CMailServer(void);
	void sendMail(const CMail &m);
	CMailIterator outbox(const char *email) const;
	CMailIterator inbox(const char *email) const;
};

#ifndef __PROGTEST__
bool matchOutput(const CMail &m,
				 const char *str) {
	ostringstream oss;
	oss << m;
	return oss.str() == str;
}

int main(void) {
	char from[100], to[100], body[1024];

	assert(CMail("john", "peter", "progtest deadline") == CMail("john", "peter", "progtest deadline"));
	assert(!(CMail("john", "peter", "progtest deadline") == CMail("john", "progtest deadline", "peter")));
	assert(!(CMail("john", "peter", "progtest deadline") == CMail("peter", "john", "progtest deadline")));
	assert(!(CMail("john", "peter", "progtest deadline") == CMail("peter", "progtest deadline", "john")));
	assert(!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "john", "peter")));
	assert(!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "peter", "john")));
	CMailServer s0;
	s0.sendMail(CMail("john", "peter", "some important mail"));
	strncpy(from, "john", sizeof(from));
	strncpy(to, "thomas", sizeof(to));
	strncpy(body, "another important mail", sizeof(body));
	s0.sendMail(CMail(from, to, body));
	strncpy(from, "john", sizeof(from));
	strncpy(to, "alice", sizeof(to));
	strncpy(body, "deadline notice", sizeof(body));
	s0.sendMail(CMail(from, to, body));
	s0.sendMail(CMail("alice", "john", "deadline confirmation"));
	s0.sendMail(CMail("peter", "alice", "PR bullshit"));
	CMailIterator i0 = s0.inbox("alice");
	assert(i0 && *i0 == CMail("john", "alice", "deadline notice"));
	assert(matchOutput(*i0, "From: john, To: alice, Body: deadline notice"));
	assert(++i0 && *i0 == CMail("peter", "alice", "PR bullshit"));
	assert(matchOutput(*i0, "From: peter, To: alice, Body: PR bullshit"));
	assert(!++i0);

	CMailIterator i1 = s0.inbox("john");
	assert(i1 && *i1 == CMail("alice", "john", "deadline confirmation"));
	assert(matchOutput(*i1, "From: alice, To: john, Body: deadline confirmation"));
	assert(!++i1);

	CMailIterator i2 = s0.outbox("john");
	assert(i2 && *i2 == CMail("john", "peter", "some important mail"));
	assert(matchOutput(*i2, "From: john, To: peter, Body: some important mail"));
	assert(++i2 && *i2 == CMail("john", "thomas", "another important mail"));
	assert(matchOutput(*i2, "From: john, To: thomas, Body: another important mail"));
	assert(++i2 && *i2 == CMail("john", "alice", "deadline notice"));
	assert(matchOutput(*i2, "From: john, To: alice, Body: deadline notice"));
	assert(!++i2);

	CMailIterator i3 = s0.outbox("thomas");
	assert(!i3);

	CMailIterator i4 = s0.outbox("steve");
	assert(!i4);

	CMailIterator i5 = s0.outbox("thomas");
	s0.sendMail(CMail("thomas", "boss", "daily report"));
	assert(!i5);

	CMailIterator i6 = s0.outbox("thomas");
	assert(i6 && *i6 == CMail("thomas", "boss", "daily report"));
	assert(matchOutput(*i6, "From: thomas, To: boss, Body: daily report"));
	assert(!++i6);

	CMailIterator i7 = s0.inbox("alice");
	s0.sendMail(CMail("thomas", "alice", "meeting details"));
	assert(i7 && *i7 == CMail("john", "alice", "deadline notice"));
	assert(matchOutput(*i7, "From: john, To: alice, Body: deadline notice"));
	assert(++i7 && *i7 == CMail("peter", "alice", "PR bullshit"));
	assert(matchOutput(*i7, "From: peter, To: alice, Body: PR bullshit"));
	assert(!++i7);

	CMailIterator i8 = s0.inbox("alice");
	assert(i8 && *i8 == CMail("john", "alice", "deadline notice"));
	assert(matchOutput(*i8, "From: john, To: alice, Body: deadline notice"));
	assert(++i8 && *i8 == CMail("peter", "alice", "PR bullshit"));
	assert(matchOutput(*i8, "From: peter, To: alice, Body: PR bullshit"));
	assert(++i8 && *i8 == CMail("thomas", "alice", "meeting details"));
	assert(matchOutput(*i8, "From: thomas, To: alice, Body: meeting details"));
	assert(!++i8);

	CMailServer s1(s0);
	s0.sendMail(CMail("joe", "alice", "delivery details"));
	s1.sendMail(CMail("sam", "alice", "order confirmation"));
	CMailIterator i9 = s0.inbox("alice");
	assert(i9 && *i9 == CMail("john", "alice", "deadline notice"));
	assert(matchOutput(*i9, "From: john, To: alice, Body: deadline notice"));
	assert(++i9 && *i9 == CMail("peter", "alice", "PR bullshit"));
	assert(matchOutput(*i9, "From: peter, To: alice, Body: PR bullshit"));
	assert(++i9 && *i9 == CMail("thomas", "alice", "meeting details"));
	assert(matchOutput(*i9, "From: thomas, To: alice, Body: meeting details"));
	assert(++i9 && *i9 == CMail("joe", "alice", "delivery details"));
	assert(matchOutput(*i9, "From: joe, To: alice, Body: delivery details"));
	assert(!++i9);

	CMailIterator i10 = s1.inbox("alice");
	assert(i10 && *i10 == CMail("john", "alice", "deadline notice"));
	assert(matchOutput(*i10, "From: john, To: alice, Body: deadline notice"));
	assert(++i10 && *i10 == CMail("peter", "alice", "PR bullshit"));
	assert(matchOutput(*i10, "From: peter, To: alice, Body: PR bullshit"));
	assert(++i10 && *i10 == CMail("thomas", "alice", "meeting details"));
	assert(matchOutput(*i10, "From: thomas, To: alice, Body: meeting details"));
	assert(++i10 && *i10 == CMail("sam", "alice", "order confirmation"));
	assert(matchOutput(*i10, "From: sam, To: alice, Body: order confirmation"));
	assert(!++i10);

	CMailServer s2;
	s2.sendMail(CMail("alice", "alice", "mailbox test"));
	CMailIterator i11 = s2.inbox("alice");
	assert(i11 && *i11 == CMail("alice", "alice", "mailbox test"));
	assert(matchOutput(*i11, "From: alice, To: alice, Body: mailbox test"));
	assert(!++i11);

	s2 = s0;
	s0.sendMail(CMail("steve", "alice", "newsletter"));
	s2.sendMail(CMail("paul", "alice", "invalid invoice"));
	CMailIterator i12 = s0.inbox("alice");
	assert(i12 && *i12 == CMail("john", "alice", "deadline notice"));
	assert(matchOutput(*i12, "From: john, To: alice, Body: deadline notice"));
	assert(++i12 && *i12 == CMail("peter", "alice", "PR bullshit"));
	assert(matchOutput(*i12, "From: peter, To: alice, Body: PR bullshit"));
	assert(++i12 && *i12 == CMail("thomas", "alice", "meeting details"));
	assert(matchOutput(*i12, "From: thomas, To: alice, Body: meeting details"));
	assert(++i12 && *i12 == CMail("joe", "alice", "delivery details"));
	assert(matchOutput(*i12, "From: joe, To: alice, Body: delivery details"));
	assert(++i12 && *i12 == CMail("steve", "alice", "newsletter"));
	assert(matchOutput(*i12, "From: steve, To: alice, Body: newsletter"));
	assert(!++i12);

	CMailIterator i13 = s2.inbox("alice");
	assert(i13 && *i13 == CMail("john", "alice", "deadline notice"));
	assert(matchOutput(*i13, "From: john, To: alice, Body: deadline notice"));
	assert(++i13 && *i13 == CMail("peter", "alice", "PR bullshit"));
	assert(matchOutput(*i13, "From: peter, To: alice, Body: PR bullshit"));
	assert(++i13 && *i13 == CMail("thomas", "alice", "meeting details"));
	assert(matchOutput(*i13, "From: thomas, To: alice, Body: meeting details"));
	assert(++i13 && *i13 == CMail("joe", "alice", "delivery details"));
	assert(matchOutput(*i13, "From: joe, To: alice, Body: delivery details"));
	assert(++i13 && *i13 == CMail("paul", "alice", "invalid invoice"));
	assert(matchOutput(*i13, "From: paul, To: alice, Body: invalid invoice"));
	assert(!++i13);

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */