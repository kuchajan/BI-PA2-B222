#ifndef __PROGTEST__
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#endif /* __PROGTEST__ */

enum class fileMode {
	UTF8,
	FIB
};

class CfileInput {
private:
	vector<unsigned char> buffer;
	fileMode mode;
	size_t currentByte;
	uint8_t currentBit;

	bool readUTF8(vector<uint32_t> & numbers);
	bool readFIB(vector<uint32_t> & numbers);

	int getTypeUTF8(unsigned char ch);
	inline uint32_t getNumUTF8(unsigned char ch, int type);
	
	inline uint32_t addToNumber(uint32_t destination, uint32_t source, uint8_t nOfBits);
public:
	CfileInput(ifstream & ifs, fileMode mod);
	bool getNumbers(vector<uint32_t> &numbers);
};

const unsigned char headerMasks[] = {
	0b10000000, // 1 byte long char
	0b11100000, // 2 byte long char
	0b11110000, // 3 byte long char
	0b11111000, // 4 byte long char
	0b11000000  // leading char
};

const unsigned char valueMasks[] = {
	0b01111111, // 1 byte long char
	0b00011111, // 2 byte long char
	0b00001111, // 3 byte long char
	0b00000111, // 4 byte long char
	0b00111111  // leading char
};

const unsigned char headers[] = {
	0b00000000, // 1 byte long char
	0b11000000, // 2 byte long char
	0b11100000, // 3 byte long char
	0b11110000, // 4 byte long char
	0b10000000  // leading char
};

int CfileInput::getTypeUTF8(unsigned char ch) {
	for (int i = 0; i < 5; i++) {
		if ((ch & headerMasks[i]) == headers[i]) {
			return i;
		}	
	}
	return -1;
}

uint32_t CfileInput::getNumUTF8(unsigned char ch, int type) {
	return (uint32_t) ch & valueMasks[type];
}

uint32_t CfileInput::addToNumber(uint32_t destination, uint32_t source, uint8_t nOfBits) {
	return (destination << nOfBits) | source;
}

bool CfileInput::readUTF8(vector<uint32_t> &numbers) {
	while (currentByte < buffer.size()) {
		unsigned char ch = buffer[currentByte];
		int type = getTypeUTF8(ch);
		
		if(type == -1 || type == 4) { //Detect error and type can't be leading char
			return false;
		}

		uint8_t NOfLeadingBytes = (uint8_t)type;
		if (currentByte + NOfLeadingBytes >= buffer.size()) { //outside of range
			return false;
		}

		//Evaluate current byte
		uint32_t number = getNumUTF8(ch, type);

		//Get leading bytes
		for (size_t i = 0; i < NOfLeadingBytes; i++) {
			unsigned char otherCh = buffer[currentByte+i+1];
			if(getTypeUTF8(otherCh) != 4) {
				return false;
			}
			uint32_t otherNum = getNumUTF8(otherCh, 4);
			number = addToNumber(number, otherNum, 6);
		}
		
		numbers.push_back(number);
		currentByte += NOfLeadingBytes + 1;
	}
	
	
	return true;
}

void fibCalcNext(uint32_t & currentFib, uint32_t & nextFib) {
	uint32_t temp = currentFib + nextFib;
	currentFib = nextFib;
	nextFib = temp;
}

bool CfileInput::readFIB(vector<uint32_t> &numbers) {
	// todo
	uint32_t currentFib = 1, nextFib = 2;
	uint32_t val = 0;
	bool previousWasOne = false;
	bool thereIsNewNum = false;
	while (currentByte < buffer.size()) {
		unsigned char ch = buffer[currentByte];
		for (int bit = 0; bit < 8; bit++) {
			bool isSet = ((ch >> bit) & 1) == 1;
			if (isSet && previousWasOne) {
				numbers.push_back(val - 1);
				val = 0;
				currentFib = 1;
				nextFib = 2;
				previousWasOne = false;
				thereIsNewNum = false;
				continue;
			}
			if (isSet) {
				previousWasOne = true;
				thereIsNewNum = true;
				val += currentFib;
			}
			else {
				previousWasOne = false;
			}
			fibCalcNext(currentFib, nextFib);
		}
		currentByte++;
	}
	
	if(thereIsNewNum) { //reading stopped too early
		return false;
	}

	return true;
}

CfileInput::CfileInput(ifstream &ifs, fileMode mod)
	: buffer(std::istreambuf_iterator<char>(ifs), (istreambuf_iterator<char>())) {
	mode = mod;
	currentByte = 0;
	currentBit = 0;
	ifs.close();
}

bool CfileInput::getNumbers(vector<uint32_t> & numbers) {
	switch (mode) {
		case fileMode::UTF8: {
			return readUTF8(numbers);
			break;
		}
		case fileMode::FIB: {
			return readFIB(numbers);
			break;
		}
	}
	return false;
}

bool readFile(const char * inFile, fileMode mode, vector<uint32_t> & numbers) {
	ifstream ifs(inFile, std::ios::binary);
	if(!ifs.is_open()) {
		return false;
	}
	CfileInput input(ifs,mode);
	
	return input.getNumbers(numbers);
}

bool utf8ToFibonacci(const char *inFile, const char *outFile) {
	vector<uint32_t> numbers;
	if (!readFile(inFile, fileMode::UTF8, numbers)) {
		return false;
	}


	// todo

	return true;
}

bool fibonacciToUtf8(const char *inFile, const char *outFile) {
	// todo
}

#ifndef __PROGTEST__

bool identicalFiles(const char *file1, const char *file2) {
	// todo
}

int main(int argc, char *argv[]) {
	assert(utf8ToFibonacci("example/src_0.utf8", "output.fib") && identicalFiles("output.fib", "example/dst_0.fib"));
	assert(utf8ToFibonacci("example/src_1.utf8", "output.fib") && identicalFiles("output.fib", "example/dst_1.fib"));
	assert(utf8ToFibonacci("example/src_2.utf8", "output.fib") && identicalFiles("output.fib", "example/dst_2.fib"));
	assert(utf8ToFibonacci("example/src_3.utf8", "output.fib") && identicalFiles("output.fib", "example/dst_3.fib"));
	assert(utf8ToFibonacci("example/src_4.utf8", "output.fib") && identicalFiles("output.fib", "example/dst_4.fib"));
	assert(!utf8ToFibonacci("example/src_5.utf8", "output.fib"));
	assert(fibonacciToUtf8("example/src_6.fib", "output.utf8") && identicalFiles("output.utf8", "example/dst_6.utf8"));
	assert(fibonacciToUtf8("example/src_7.fib", "output.utf8") && identicalFiles("output.utf8", "example/dst_7.utf8"));
	assert(fibonacciToUtf8("example/src_8.fib", "output.utf8") && identicalFiles("output.utf8", "example/dst_8.utf8"));
	assert(fibonacciToUtf8("example/src_9.fib", "output.utf8") && identicalFiles("output.utf8", "example/dst_9.utf8"));
	assert(fibonacciToUtf8("example/src_10.fib", "output.utf8") && identicalFiles("output.utf8", "example/dst_10.utf8"));
	assert(!fibonacciToUtf8("example/src_11.fib", "output.utf8"));

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
