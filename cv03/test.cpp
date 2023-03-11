#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class Record {
	private:
	string name;
	string surname;
	int number;
	public:
	bool setRecord(const string & line) {
		istringstream iss(line);
		iss >> name >> surname >> setw(9) >> number;
		if (!iss.eof()) {
			iss >> ws;
		}
		return !iss.fail() && iss.eof() && number >= 100000000;
	}
	bool cmpName(string otherName) const {
		return otherName == name || otherName == surname;
	}
	void writeRecord(ostream & out) const {
		out << name << " " << surname << " " << number << "\n";
	}
};

bool getRecords (ifstream & ifs, vector<Record> & records) {
	string line;
	for(size_t i = 0; getline(ifs,line), line != "" && !ifs.eof();i++) { //TODO: chybí prázdná řádka za seznamem čísel.
		records.push_back(Record());
		if(!(records[i].setRecord(line))) {
			return false;
		}
	}
	
	return (line == "");
}

void findRecords (ifstream & ifs, vector<Record> & records, ostream & out) {
	string line;
	while (getline(ifs,line), !ifs.fail()) {
		int count = 0;
		for (size_t i = 0; i < records.size(); i++) {
			if(records[i].cmpName(line)) {
				records[i].writeRecord(out);
				count++;
			}
		}
		out << "-> " << count << "\n";
	}
	
	
	
	//return true;
}

bool report ( const string & fileName, ostream & out )
{
	vector<Record> records;
	ifstream ifs = ifstream(fileName);
	if (!getRecords(ifs,records)) {
		return false;
	}
	
	findRecords(ifs,records,out);
	return true;
}

#ifndef __PROGTEST__
int main ()
{
  ostringstream oss;
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  return 0;
}
#endif /* __PROGTEST__ */
