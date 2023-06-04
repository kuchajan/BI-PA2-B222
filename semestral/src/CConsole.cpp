#include "CConsole.hpp"

void CConsole::output() {
	for (auto iter = m_art.begin(); iter != m_art.end(); ++iter) {
		cout << (*iter);
		if (next(iter) != m_art.end()) {
			cout << "\n";
		}
	}
}

shared_ptr<COutput> CConsole::clone() {
	return make_shared<CConsole>(*this);
}
