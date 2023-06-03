#include "CFile.hpp"

CFile::CFile(const char *path) : m_path(path) {}

shared_ptr<COutput> CFile::clone() {
	return make_shared<CFile>(*this);
}

void CFile::output() {
	int i = 1;
	for (auto art : m_art) {
		string path = m_path + to_string(i) + ".txt";
		ofstream of(path);
		of << art;
		if (!of) {
			throw logic_error(string("Couldn't write to file ") + path);
		}
		++i;
	}
}
