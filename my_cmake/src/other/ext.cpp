#include <iostream>

extern int abc;
int ijk = 22;

void print() {
	std::cout << "[extern]abc=" << abc << "; ijk=" << ijk << std::endl;
}

