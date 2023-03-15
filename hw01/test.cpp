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

class CFileInput {
private:
	ifstream ifs;
	fileMode mode;

	// UTF8 methods
	bool readUTF8(vector<uint32_t> &numbers);
	int getTypeUTF8(unsigned char ch);
	inline uint32_t getNumUTF8(unsigned char ch, int type);
	inline uint32_t addToNumber(uint32_t destination, uint32_t source, uint8_t nOfBits);

	// FIB methods
	bool readFIB(vector<uint32_t> &numbers);

public:
	CFileInput(const char *file, fileMode mod);
	bool getNumbers(vector<uint32_t> &numbers);
};

const unsigned char headerMasks[] = {
	0b10000000, // 1 byte long char
	0b11100000, // 2 byte long char
	0b11110000, // 3 byte long char
	0b11111000, // 4 byte long char
	0b11000000	// leading char
};

const unsigned char valueMasks[] = {
	0b01111111, // 1 byte long char
	0b00011111, // 2 byte long char
	0b00001111, // 3 byte long char
	0b00000111, // 4 byte long char
	0b00111111	// leading char
};

const unsigned char headers[] = {
	0b00000000, // 1 byte long char
	0b11000000, // 2 byte long char
	0b11100000, // 3 byte long char
	0b11110000, // 4 byte long char
	0b10000000	// leading char
};

int CFileInput::getTypeUTF8(unsigned char ch) {
	for (int i = 0; i < 5; i++) {
		if ((ch & headerMasks[i]) == headers[i]) {
			return i;
		}
	}
	return -1;
}

uint32_t CFileInput::getNumUTF8(unsigned char ch, int type) {
	return (uint32_t)ch & valueMasks[type];
}

uint32_t CFileInput::addToNumber(uint32_t destination, uint32_t source, uint8_t nOfBits) {
	return (destination << nOfBits) | source;
}

bool CFileInput::readUTF8(vector<uint32_t> &numbers) {
	unsigned char ch;
	while (ch = ifs.get(), ifs.good()) {
		int type = getTypeUTF8(ch);

		if (type == -1 || type == 4) { // Byte isn't UTF8 or is leading
			return false;
		}

		// Evaluate current byte
		uint32_t number = getNumUTF8(ch, type);

		// Get leading bytes
		for (uint8_t i = 0; i < type /*type also gives out number of leading bytes*/; i++) {
			unsigned char otherCh = ifs.get();
			if (ifs.eof() || getTypeUTF8(otherCh) != 4) { // outside of range or not leading byte
				return false;
			}
			uint32_t otherNum = getNumUTF8(otherCh, 4);
			number = addToNumber(number, otherNum, 6);
		}

		numbers.push_back(number);
	}
	
	return true;
}

const uint32_t fibonacciSeq[] = {
	      1,       2,       3,       5,       8,
	     13,      21,      34,      55,      89,
	    144,     233,     377,     610,     987,
	   1597,    2584,    4181,    6765,
	  10946,   17711,   28657,   46368,   75025,
	 121393,  196418,  317811,  514229,  832040,
	1346269
};

bool CFileInput::readFIB(vector<uint32_t> &numbers) {
	size_t fibIndex = 0;
	uint32_t val = 0;
	bool previousWasOne = false;
	bool thereIsNewNum = false;
	unsigned char ch;
	while (ch = ifs.get(), ifs.good()) {
		for (uint8_t bit = 0; bit < 8; bit++) {
			bool isSet = ((ch >> bit) & 1) == 1;
			if (isSet && previousWasOne) {
				if (val - 1 > 2097151) {
					return false; // normally, I would put this into the writeUTF8 function, but we only use these two encodings
				}
				numbers.push_back(val - 1);
				val = 0;
				fibIndex = 0;
				previousWasOne = false;
				thereIsNewNum = false;
				continue;
			}
			if (fibIndex > (sizeof(fibonacciSeq) / sizeof(uint32_t)) - 1) { //number is too big
				return false;
			}
			if (isSet) {
				previousWasOne = true;
				thereIsNewNum = true;
				val += fibonacciSeq[fibIndex];
			} else {
				previousWasOne = false;
			}

			fibIndex++;
		}
	}

	return !(thereIsNewNum || !ifs.eof()); // unfinished number or reading stopped too early
}

CFileInput::CFileInput(const char *file, fileMode mod) : ifs(file, std::ios::binary) {
	mode = mod;
}

bool CFileInput::getNumbers(vector<uint32_t> &numbers) {
	if (!ifs.is_open()) {
		return false;
	}
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

bool readFile(const char *inFile, fileMode mode, vector<uint32_t> &numbers) {
	CFileInput input(inFile, mode);
	return input.getNumbers(numbers);
}

class CFileOutput {
private:
	ofstream ofs;
	fileMode mode;

	// UTF8 methods
	int getNumType(uint32_t number);
	unsigned char getUTF8Byte(uint32_t &number, int type);
	bool numberToUTF8(uint32_t number, vector<unsigned char> &toOutput);
	bool writeUTF8(vector<uint32_t> &numbers);

	// FIB methods
	void getFibCode(uint32_t number, vector<unsigned char> &bytes, int &bitCount);
	void bitshiftWithOverflow(vector<unsigned char> &bytes);
	bool writeFIB(vector<uint32_t> &numbers);

public:
	CFileOutput(const char *file, fileMode mod);
	bool write(vector<uint32_t> &numbers);
};

int CFileOutput::getNumType(uint32_t number) {
	if (number <= 127) {
		return 0; // 1 byte - 1 header, 0 leading
	}
	if (number <= 2047) {
		return 1; // 2 bytes - 1 header, 1 leading
	}
	if (number <= 65537) {
		return 2; // 3 bytes - 1 header, 2 leading
	}
	if (number <= 2097151) {
		return 3; // 4 bytes - 1 header, 3 leading
	}
	return -1; // number too large
}

unsigned char CFileOutput::getUTF8Byte(uint32_t &number, int type) {
	unsigned char ch = headers[type] | (number & valueMasks[type]);
	int bitsToShift = -1;
	switch (type) {
		case 0: {
			bitsToShift = 7;
			break;
		}
		case 1: {
			bitsToShift = 5;
			break;
		}
		case 2: {
			bitsToShift = 4;
			break;
		}
		case 3: {
			bitsToShift = 3;
			break;
		}
		case 4: {
			bitsToShift = 6;
			break;
		}
		default: {
			return 0xFF; // This shouldn't happen
		}
	}

	number = number >> bitsToShift;

	return ch;
}

bool CFileOutput::numberToUTF8(uint32_t number, vector<unsigned char> &toOutput) {
	int type = getNumType(number);
	if (type == -1) {
		return false;
	}

	toOutput.resize(type + 1);

	for (int i = type; i >= 0; i--) {
		unsigned char ch;
		if (i == 0 || type == 0) {
			ch = getUTF8Byte(number, type);
		} else {
			ch = getUTF8Byte(number, 4);
		}
		toOutput[i] = ch;
	}

	return true;
}

bool CFileOutput::writeUTF8(vector<uint32_t> &numbers) {
	for (size_t i = 0; i < numbers.size(); i++) {
		vector<unsigned char> toOutput;
		if (!numberToUTF8(numbers[i], toOutput)) {
			return false;
		}
		ofs.write((char *)toOutput.data(), toOutput.size());
		if (ofs.bad()) {
			return false;
		}
	}
	return true;
}

void CFileOutput::bitshiftWithOverflow(vector<unsigned char> &bytes) {
	bool overflow = false;
	for (size_t i = 0; i < bytes.size(); i++) {
		bool overflowNext = (bytes[i] & 0b10000000) != 0;
		bytes[i] = overflow ? (bytes[i] << 1) | 1 : bytes[i] << 1;
		overflow = overflowNext;
	}
	if (overflow) {
		bytes.push_back(1);
	}
}

void CFileOutput::getFibCode(uint32_t number, vector<unsigned char> &bytes, int &bitCount) {
	int index = -1;
	bytes.push_back(0b11);

	for (int i = (sizeof(fibonacciSeq) / sizeof(uint32_t)) - 1; i >= 0; i--) {
		if (fibonacciSeq[i] <= number) {
			index = i;
			number -= fibonacciSeq[i];
			break;
		}
	}

	bitCount = index + 2; // index + 1 because we're counting from zero + 1 because of the end bit

	for (int i = index - 1; i >= 0; i--) {
		bitshiftWithOverflow(bytes);
		if (fibonacciSeq[i] <= number) {
			bytes[0] |= 0b1;
			number -= fibonacciSeq[i];
		}
	}
}

bool CFileOutput::writeFIB(vector<uint32_t> &numbers) {
	int bitsToShift = 0;
	unsigned char lastByte = 0;

	for (size_t i = 0; i < numbers.size(); i++) {
		// Because the number can exceed a single byte, I save it into a vector
		// I get the current number's bytes in fibonacci
		vector<unsigned char> toOutput;
		int bitCount;
		getFibCode(numbers[i] + 1, toOutput, bitCount);

		// Bitshift with overflow, so I won't overwrite the lastByte
		for (int j = 0; j < bitsToShift; j++, bitCount++) {
			bitshiftWithOverflow(toOutput);
		}

		// or the lastByte with the first byte of toOutput
		lastByte |= toOutput[0];

		// write the last byte (if it's finished)
		if (bitCount >= 8) {
			ofs << lastByte;
		}

		// write the remaining except for the last one
		for (size_t j = 1; j < toOutput.size() - 1; j++) {
			ofs << toOutput[j];
		}

		// if the last byte was written,
		// we need to change what the last byte is now
		// but if the last byte of the output vector is
		// also full and it's not the last byte, we need to write it, and thus the
		// last byte is 0
		// else, last byte is the last byte of the vector
		if (bitCount >= 8) { // if last byte was written
			// change the last byte
			lastByte = toOutput[toOutput.size() - 1];
			if ((bitCount % 8) == 0) {
				if (bitCount != 8) {
					ofs << lastByte;
				}
				lastByte = 0;
			}
		}
		// reset the bitsToShift
		bitsToShift = bitCount % 8;
	}
	if (lastByte > 0) {
		ofs << lastByte;
	}
	return true;
}

CFileOutput::CFileOutput(const char *file, fileMode mod) : ofs(file, std::ios::binary) {
	mode = mod;
}

bool CFileOutput::write(vector<uint32_t> &numbers) {
	if (!ofs.is_open()) {
		return false;
	}

	switch (mode) {
		case fileMode::UTF8: {
			return writeUTF8(numbers);
		}
		case fileMode::FIB: {
			return writeFIB(numbers);
		}
	}

	return false;
}

bool writeFile(const char *outFile, fileMode mode, vector<uint32_t> &numbers) {
	CFileOutput output(outFile, mode);
	return output.write(numbers);
}

bool utf8ToFibonacci(const char *inFile, const char *outFile) {
	vector<uint32_t> numbers;
	if (!readFile(inFile, fileMode::UTF8, numbers)) {
		return false;
	}

	if (!writeFile(outFile, fileMode::FIB, numbers)) {
		return false;
	}

	return true;
}

bool fibonacciToUtf8(const char *inFile, const char *outFile) {
	vector<uint32_t> numbers;
	if (!readFile(inFile, fileMode::FIB, numbers)) {
		return false;
	}

	if (!writeFile(outFile, fileMode::UTF8, numbers)) {
		return false;
	}

	return true;
}

#ifndef __PROGTEST__

bool identicalFiles(const char *filename1, const char *filename2) {
	ifstream file1(filename1, std::ios::binary);
	if (!file1.is_open()) {
		return false;
	}
	ifstream file2(filename2, std::ios::binary);
	if (!file2.is_open()) {
		return false;
	}

	istreambuf_iterator<char> f1(file1);
	istreambuf_iterator<char> f2(file2);

	return equal(f1, istreambuf_iterator<char>(), f2);
}

int main(void) {
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

	// utf8ToFibonacci("example/myTests/src_0.utf8","example/myTests/src_1.fib");
	assert(utf8ToFibonacci("example/myTests/src_0.utf8", "output.fib") && identicalFiles("output.fib", "example/myTests/src_1.fib"));	// max utf8
	assert(fibonacciToUtf8("example/myTests/src_1.fib", "output.utf8") && identicalFiles("output.utf8", "example/myTests/src_0.utf8")); // max utf8
	assert(!fibonacciToUtf8("example/myTests/src_2.fib", "output.utf8"));																// max utf8 + 1
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
