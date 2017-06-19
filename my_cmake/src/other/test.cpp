#include "Rectange.h"
#include <iostream>

//class b {
//	char i;
//	char k;
//	int d;
//};

int main(void) {
	Point a(1.0, 2.0);
	Point b(3.0, 4.1);
	Rectange myRect(a, b);
	Point temp = myRect.TopRightPoint();
	temp.PrintInfo();
	temp = myRect.TopLeftPoint();
	temp.PrintInfo();
	temp = myRect.BottomRightPoint();
	temp.PrintInfo();
	temp = myRect.BottomLeftPoint();
	temp.PrintInfo();
	std::cout << "Area = " << myRect.Area() << std::endl;

	return 0;
}
