#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "Usage: " << argv[0] << " -flags" << endl;
		return 0;
	}

	cout << "Hello World!" << endl;
	return 0;
}