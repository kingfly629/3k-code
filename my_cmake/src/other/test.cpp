#include "Rectange.h"
#include "myfriend.h"
#include <memory>
#include <iostream>

using namespace kkk;

#define FUNC(X,Y) X*Y-Y
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

	std::cout << "=========friend function=============" << std::endl;

	//2- friend function
	myfriend obj(3);
	friend_PrintInfo(obj);

	std::cout << "==========friend class============" << std::endl;

	//3- friend class
	//myfriend obj(3);
	classB b(obj);
	b.test();

	std::cout << "===========define===========" << std::endl;
	//4- define
	std::cout << "FUNC(x,y)" << FUNC(1 + 9, 5 - 3) << std::endl;

	std::cout << "==========auto_ptr()============" << std::endl;
	//5- auto_ptr
	std::auto_ptr<myfriend> x = std::auto_ptr<myfriend>(new myfriend(300));
	//std::auto_ptr<myfriend> x(new myfriend(300));
	myfriend *y = x.get();//x.release();//x.get();
	std::cout<<x.get()<<std::endl;
	std::cout<<y<<std::endl;
	y->test();
	myfriend *z = new myfriend;
	//delete y;
	return 0;
}
