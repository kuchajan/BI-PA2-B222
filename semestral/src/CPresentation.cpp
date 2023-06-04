#include "CPresentation.hpp"

void CPresentation::showArt(size_t idx) {
	clear();
	if (idx >= m_art.size()) {
		endwin();
		throw logic_error("CPresentation::showArt: index is out of bounds");
	}
	ostringstream oss;
	oss << m_art[idx];
	addstr(oss.str().c_str());
	refresh();
}

void CPresentation::output() {
	// initialize ncurses
	initscr();
	noecho();
	keypad(stdscr, true);
	cbreak();
	timeout(2000);

	bool exit = false;
	bool stopped = false;
	size_t idx = 0;
	// first show
	showArt(idx);

	while (!exit) {
		switch (wgetch(stdscr)) {
			case KEY_LEFT: {
				if (idx == 0) {
					idx = m_art.size() - 1;
				} else {
					--idx;
				}
				showArt(idx);
				break;
			}
			case KEY_RIGHT: {
				if (++idx == m_art.size()) {
					idx = 0;
				}
				showArt(idx);
				break;
			}
			case ' ': {
				if (stopped) {
					timeout(2000);
				} else {
					timeout(-1);
				}
				stopped = !stopped;

				break;
			}
			case 'q': {
				exit = true;
				break;
			}
			case -1: {
				if (++idx == m_art.size()) {
					idx = 0;
				}
				showArt(idx);
				break;
			}
		}
	}

	endwin();
}

shared_ptr<COutput> CPresentation::clone() {
	return make_shared<CPresentation>(*this);
}
