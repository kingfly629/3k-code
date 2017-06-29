#include <iostream>
#include <string.h>
using namespace std;

int abc = 10;
int ijk = 20;
extern void print();

int main(void) {
	cout << "abc=" << abc << "; ijk=" << ijk << endl;
	print();
	return 0;
}
