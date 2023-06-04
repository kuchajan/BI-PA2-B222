#pragma once

#include "COutput.hpp"
#include <fstream>

/// @brief An outputter, which outputs the generated art in text files numbered from 0 to n
class CFile : public COutput {
private:
	/// @brief The path where to save the text files
	string m_path;

public:
	/// @brief Constructs the outputter and copies the path
	/// @param path The path
	CFile(const char *path);
	virtual shared_ptr<COutput> clone();
	virtual void output() override;
};