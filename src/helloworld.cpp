#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
	cout << "Hello, world!" << endl;
	cout << "Arguments:" << endl;
	for (int i = 0; i<argc; ++i) {
		cout << " " << argv[i] << endl;
	}
	return 0;
}
