#include "CConsole.hpp"

void CConsole::output() {
	for (auto art : m_art) {
		cout << art << "\n\n";
	}
}

shared_ptr<COutput> CConsole::clone() {
	return make_shared<CConsole>(*this);
}
