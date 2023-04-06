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
	CShared_ptr(const T &);		// constructor
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
	os << "From: " << m.m_From << ", To: " << m.m_To << ", Body: " << m.m_Body;
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
	return *this;
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

bool CPerson::operator==(const CPerson &other) const {
	return m_address == other.m_address;
}

bool CPerson::operator!=(const CPerson &other) const {
	return !(m_address == other.m_address);
}

bool CPerson::operator<(const CPerson &other) const {
	return m_address < other.m_address;
}

bool CPerson::operator>(const CPerson &other) const {
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

template <class T>
class CAVLTree {
private:
	struct SAVLNode {
		// variables
		SAVLNode *m_Parent;
		SAVLNode *m_LeftChild;
		SAVLNode *m_RightChild;
		T *m_Value;

		// methods
		// constructor
		SAVLNode(const T &value) {
			m_Value = new T(value);
			m_Parent = nullptr;
			m_LeftChild = nullptr;
			m_RightChild = nullptr;
		}

		// balance factor methods
		ssize_t getNOfChildren() {
			ssize_t cnt = 0;
			if (m_LeftChild != nullptr) {
				cnt += m_LeftChild->getNOfChildren() + 1;
			}
			if (m_RightChild != nullptr) {
				cnt += m_RightChild->getNOfChildren() + 1;
			}
			return cnt;
		}

		ssize_t getBalanceFactor() {
			ssize_t bal = 0;
			if (m_LeftChild != nullptr) {
				bal += m_LeftChild->getNOfChildren() + 1;
			}
			if (m_RightChild != nullptr) {
				bal -= m_RightChild->getNOfChildren() + 1;
			}
			return bal;
		}

		// rotations
		SAVLNode *rotate_Left() {
			SAVLNode *newRoot = m_RightChild;
			m_RightChild = newRoot->m_LeftChild;
			if (newRoot->m_LeftChild != nullptr) {
				newRoot->m_LeftChild->m_Parent = this;
			}
			newRoot->m_LeftChild = this;
			newRoot->m_Parent = this->m_Parent;

			m_Parent = newRoot;
			return newRoot;
		}

		SAVLNode *rotate_Right() {
			SAVLNode *newRoot = m_LeftChild;
			m_LeftChild = newRoot->m_RightChild;
			if (newRoot->m_RightChild != nullptr) {
				newRoot->m_RightChild->m_Parent = this;
			}
			newRoot->m_RightChild = this;
			newRoot->m_Parent = this->m_Parent;

			m_Parent = newRoot;
			return newRoot;
		}

		// insertion
		SAVLNode *recursiveInsert(SAVLNode *toInsert) {
			// binary insertion
			if (*(toInsert->m_Value) < *(m_Value)) {
				if (m_LeftChild != nullptr) {
					m_LeftChild = m_LeftChild->recursiveInsert(toInsert);
				} else {
					m_LeftChild = toInsert;
					m_LeftChild->m_Parent = this;
				}
			} else if (*(toInsert->m_Value) > *(m_Value)) {
				if (m_RightChild != nullptr) {
					m_RightChild = m_RightChild->recursiveInsert(toInsert);
				} else {
					m_RightChild = toInsert;
					m_RightChild->m_Parent = this;
				}
			} else {
				throw logic_error("Attempted to insert a duplicate value");
			}

			// rebalancing
			ssize_t balanceFactor = getBalanceFactor();
			if (balanceFactor > 1) {
				// rotate left the left child if neccessary
				if (m_LeftChild != nullptr && *(toInsert->m_Value) > *(m_LeftChild->m_Value)) {
					m_LeftChild = m_LeftChild->rotate_Left();
				}
				return rotate_Right();
			}
			if (balanceFactor < -1) {
				// rotate right the right child if neccessary
				if (m_RightChild != nullptr && *(toInsert->m_Value) < *(m_RightChild->m_Value)) {
					m_RightChild = m_RightChild->rotate_Right();
				}
				return rotate_Left();
			}
			return this;
		}

		/*// iterator helpers
		SAVLNode *getLeftMost() {
			if (m_LeftChild != nullptr) {
				return m_LeftChild->getLeftMost();
			}
			return this;
		}*/
	};

	/*
	=====================================================================================================
	=====================================================================================================
	=====================================================================================================
	=====================================================================================================
	=====================================================================================================
	*/

	typedef SAVLNode Node;
	Node *m_Root;

	void deconstructRecursive(Node *toDelete) {
		if (toDelete->m_LeftChild != nullptr) {
			deconstructRecursive(toDelete->m_LeftChild);
		}
		if (toDelete->m_RightChild != nullptr) {
			deconstructRecursive(toDelete->m_RightChild);
		}
		delete toDelete->m_Value;
		delete toDelete;
	}

	Node *copyRecursive(Node *toCopyFrom) {
		if (toCopyFrom == nullptr) {
			return nullptr;
		}

		Node *newNode = new Node(*(toCopyFrom->m_Value)); // will this truly create a copy?

		// left child
		newNode->m_LeftChild = copyRecursive(toCopyFrom->m_LeftChild);
		if (newNode->m_LeftChild != nullptr) {
			newNode->m_LeftChild->m_Parent = newNode;
		}

		// right child
		newNode->m_RightChild = copyRecursive(toCopyFrom->m_RightChild);
		if (newNode->m_RightChild != nullptr) {
			newNode->m_RightChild->m_Parent = newNode;
		}

		// done
		return newNode;
	}

public:
	CAVLTree() {
		m_Root = nullptr;
	}
	CAVLTree(const CAVLTree &other) {
		m_Root = copyRecursive(other.m_Root);
	}
	// copy recursive
	~CAVLTree() {
		if (m_Root != nullptr) {
			deconstructRecursive(m_Root);
		}
	}

	class CAVLIterator {
	private:
		Node *m_Node;

	public:
		/*CAVLIterator() {
			m_Node = m_Root->getLeftMost();
		}*/

		CAVLIterator(Node *node) : m_Node(node) {}

		T *operator*() {
			return m_Node == nullptr ? nullptr : m_Node->m_Value;
		}
	};

	CAVLIterator find(const T *toFind) const {
		Node *current = m_Root;
		while (current != nullptr && (*current->m_Value) != *toFind) {
			current = *toFind > (*current->m_Value) ? current->m_RightChild : current->m_LeftChild;
		}
		return CAVLIterator(current);
	}

	void insert(const T &toInsert) {
		Node *newNode = new Node(toInsert);
		if (m_Root == nullptr) {
			m_Root = newNode;
			return;
		}
		m_Root = m_Root->recursiveInsert(newNode);
	}

	void rootSwap(CAVLTree &other) {
		Node *temp = m_Root;
		m_Root = other.m_Root;
		other.m_Root = temp;
	}
};

class CMailServer {
private:
	CAVLTree<CPerson> m_addresses;

public:
	CMailServer(void);
	CMailServer(const CMailServer &src);
	CMailServer &operator=(CMailServer src);
	~CMailServer(void);
	void sendMail(const CMail &m);
	CMailIterator outbox(const char *email) const;
	CMailIterator inbox(const char *email) const;

	CAVLTree<CPerson>::CAVLIterator find(const char *mail) const;
	CAVLTree<CPerson>::CAVLIterator findOrInsert(const MyString &email);
};

CMailServer::CMailServer() {}

CMailServer::CMailServer(const CMailServer &src) : m_addresses(src.m_addresses) {}

CMailServer &CMailServer::operator=(CMailServer src) {
	m_addresses.rootSwap(src.m_addresses);
	return *this;
}

CMailServer::~CMailServer() {}

CAVLTree<CPerson>::CAVLIterator CMailServer::find(const char *mail) const {
	MyString email(mail);
	CPerson tempPerson(email);

	return m_addresses.find(&tempPerson);
}

CAVLTree<CPerson>::CAVLIterator CMailServer::findOrInsert(const MyString &email) {
	CPerson tempPerson(email);

	auto toReturn = m_addresses.find(&tempPerson);
	if (*toReturn != nullptr) {
		return toReturn;
	}

	m_addresses.insert(tempPerson);
	// todo: maybe i could return the iterator of where I saved it
	toReturn = m_addresses.find(&tempPerson);
	return toReturn;
}

void CMailServer::sendMail(const CMail &m) {
	CShared_ptr<CMail> newMail(m);
	auto sendFrom = findOrInsert(m.m_From);
	auto sendTo = findOrInsert(m.m_To);
	(*sendFrom)->send(newMail);
	(*sendTo)->receive(newMail);
}

CMailIterator CMailServer::outbox(const char *email) const {
	auto iter = find(email);
	return *iter == nullptr ? CMailIterator() : (*iter)->outbox();
}

CMailIterator CMailServer::inbox(const char *email) const {
	auto iter = find(email);
	return *iter == nullptr ? CMailIterator() : (*iter)->inbox();
}

#ifndef __PROGTEST__
bool matchOutput(const CMail &m,
				 const char *str) {
	ostringstream oss;
	oss << m;
	return oss.str() == str;
}

void test0();
void test1();

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

	test0();
	test1();

	return EXIT_SUCCESS;
}

void testSpecificIterator(CMailIterator &iter, const bool &check) {
	for (int i = 0; i < 2 && check; i++) {
		assert(!iter == false);
		assert((bool)iter == true);
		// cout << *iter << endl;
		++iter;
	}
	assert(!iter == true);
	assert((bool)iter == false);
}

void testIterators(const CMailServer &server, const char *email, const bool &checkIn, const bool &checkOut) {
	// cout << "Checking: " << email << endl;

	CMailIterator iterIn = server.inbox(email);
	CMailIterator iterOut = server.outbox(email);
	testSpecificIterator(iterIn, checkIn);
	testSpecificIterator(iterOut, checkOut);
}

void test0() {
	CMailServer server;
	// comparing
	server.sendMail(CMail("Ac", "aC", "My very special email to you"));
	server.sendMail(CMail("AC", "ac", "My very special email to you"));
	server.sendMail(CMail("BB", "bb", "My very special email to you"));
	server.sendMail(CMail("Bb", "bB", "My very special email to you"));
	// length
	server.sendMail(CMail("A", "a", "My very special email to you"));
	server.sendMail(CMail("B", "b", "My very special email to you"));
	server.sendMail(CMail("C", "c", "My very special email to you"));

	// null byte
	server.sendMail(CMail("", "", "Making first years go insane 101"));

	// repeat
	// comparing
	server.sendMail(CMail("Ac", "aC", "My very special email to you"));
	server.sendMail(CMail("AC", "ac", "My very special email to you"));
	server.sendMail(CMail("BB", "bb", "My very special email to you"));
	server.sendMail(CMail("Bb", "bB", "My very special email to you"));
	// length
	server.sendMail(CMail("A", "a", "My very special email to you"));
	server.sendMail(CMail("B", "b", "My very special email to you"));
	server.sendMail(CMail("C", "c", "My very special email to you"));

	// null byte
	server.sendMail(CMail("", "", "Making first years go insane 101"));

	testIterators(server, "Ac", false, true);
	testIterators(server, "aC", true, false);
	testIterators(server, "AC", false, true);
	testIterators(server, "ac", true, false);
	testIterators(server, "BB", false, true);
	testIterators(server, "bb", true, false);
	testIterators(server, "Bb", false, true);
	testIterators(server, "bB", true, false);
	testIterators(server, "A", false, true);
	testIterators(server, "a", true, false);
	testIterators(server, "B", false, true);
	testIterators(server, "b", true, false);
	testIterators(server, "C", false, true);
	testIterators(server, "c", true, false);
	testIterators(server, "", true, true);

	testIterators(server, "nonexistant", false, false);
}

void test1() {
	CMailServer server;
	
	// comparing
	CMail mail1("Ac", "aC", "My very special email to you");
	CMail mail2("AC", "ac", "My very special email to you");
	CMail mail3("BB", "bb", "My very special email to you");
	CMail mail4("Bb", "bB", "My very special email to you");
	// length
	CMail mail5("A", "a", "My very special email to you");
	CMail mail6("B", "b", "My very special email to you");
	CMail mail7("C", "c", "My very special email to you");
	CMail mail8("", "", "Making first years go insane 101");

	// send them
	server.sendMail(mail1);
	server.sendMail(mail2);
	server.sendMail(mail3);
	server.sendMail(mail4);
	server.sendMail(mail5);
	server.sendMail(mail6);
	server.sendMail(mail7);
	server.sendMail(mail8);
	// repeat
	server.sendMail(mail1);
	server.sendMail(mail2);
	server.sendMail(mail3);
	server.sendMail(mail4);
	server.sendMail(mail5);
	server.sendMail(mail6);
	server.sendMail(mail7);
	server.sendMail(mail8);
}
#endif /* __PROGTEST__ */