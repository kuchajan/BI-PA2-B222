#pragma once

#include "COutput.hpp"
#include <fstream>

class CFile : public COutput {
private:
	string m_path;

public:
	CFile(const char *path);
	virtual shared_ptr<COutput> clone();
	virtual void output() override;
};