#include "Rectange.h"
#include "myfriend.h"
#include <iostream>

using namespace kkk;
//class b {
//	char i;
//	char k;
//	int d;
//};

namespace kkk {
	void friend_PrintInfo(const myfriend& obj) {
		std::cout << "here friend function:" << obj.x << std::endl;
	}
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

	std::cout << "======================" << std::endl;
	
	//2- friend function
	myfriend obj(3);
	friend_PrintInfo(obj);

	std::cout << "======================" << std::endl;
	
	//3- friend class
	//myfriend obj(3);
	classB b(obj);
	b.test();
	return 0;
}
