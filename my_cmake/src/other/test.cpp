#include "Rectange.h"
#include "myfriend.h"
#include <iostream>

//class b {
//	char i;
//	char k;
//	int d;
//};

void PrintInfo(const myfriend& obj) {
	std::cout << "here friend function.:" << obj.x << std::endl;
}

int main(void) {
	//1- construction
	//	Point a(1.0, 2.0);
	//	Point b(3.0, 4.1);
	//	Rectange myRect_tmp(a, b);
	//	Rectange myRect(myRect_tmp);
	//	//myRect = myRect_tmp;
	//	Point temp = myRect.TopRightPoint();
	//	temp.PrintInfo();
	//	myRect.TopLeftPoint();
	//	temp.PrintInfo();
	//	temp = myRect.BottomRightPoint();
	//	temp.PrintInfo();
	//	temp = myRect.BottomLeftPoint();
	//	temp.PrintInfo();
	//	std::cout << "Area = " << myRect.Area() << std::endl;

	//2- friend function
	myfriend obj(3);
	PrintInfo(obj);

	return 0;
}
