#include "Rectange.h"
#include "myfriend.h"
#include <memory>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

using namespace kkk;

#define FUNC(X,Y) X*Y-Y

typedef unsigned int uint8;
typedef unsigned long uint64;
typedef int (*CallBack)(int, int);

int division(int a, int b) {
	std::cout << "a=" << a << "; b=" << b << std::endl;
	int c1 = a / b;
	float c2 = a / b;
	double c3 = a / b;
	std::cout << "(int)a/b=" << c1 << std::endl;
	std::cout << "(int)a/b=" << (int) a / b << std::endl;
	std::cout << "(float)a/b=" << c2 << std::endl;
	std::cout << "(float)a/b=" << (float) a / b << std::endl;
	std::cout << "(double)a/b=" << c3 << std::endl;
	std::cout << "(double)a/b=" << (double) a / b << std::endl;
}

void test(int a, int b, CallBack _callback) {
	_callback(a, b);
}

namespace kkk {

	void friend_PrintInfo(const myfriend& obj) {
		std::cout << "here friend function:" << obj.x << std::endl;
	}
}

char* my_strcpy(char *dest, const char *src) {
	assert(sizeof (dest) >= sizeof (src));
	while (*src != '\0') {
		*dest++ = *src++;
	}
	return dest;
}

char* my_str_replace(const char *src, const char *sub, const char* replace) {
	char *result = (char *) malloc(sizeof (char) * (sizeof (src) - sizeof (sub) + sizeof (replace)));
	const char *temp = strstr(src, sub);
	if (!temp) {
		return NULL;
	}
	std::cout << "temp:" << temp << std::endl;
	//strncpy(result, src, temp - src);
	//result[temp-src] = '\0';
	strncat(result, src, temp - src);
	std::cout << "cur 1:" << strlen(result) << std::endl;
	char *q = strcat(result, replace);
	std::cout << "cur 2:" << strlen(result) << std::endl;
	char *p = strcat(result, temp + strlen(sub));
	std::cout << "cur 3:" << strlen(result) << std::endl;
	//result[strlen(result)] = '\0';

	return p;
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
	myfriend *y = x.get(); //x.release();//x.get();
	std::cout << x.get() << std::endl;
	std::cout << y << std::endl;
	y->test();
	//myfriend *z = new myfriend;
	//delete z;

	std::cout << "==========str_replace============" << std::endl;
	const char *src = "wangjing jiang";
	const char *sub = "jing";
	const char *replace = "jin";
	std::cout << "before replace:" << src << std::endl;
	char *p = my_str_replace(src, sub, replace);
	std::cout << "after replace:" << p << std::endl;
	free(p);

	std::cout << "==========strcpy============" << std::endl;
	char *dest = new char [sizeof (src)];
	std::cout << "src:" << src << std::endl;
	my_strcpy(dest, src);
	std::cout << "dest:" << dest << std::endl;
	delete [] dest;

	std::cout << "==========typedef function & callback============" << std::endl;
	//int (*p)(int,char) = &ttt;
	//CallBack func = division;
	//pq(1,'c');
	test(3, 2, division);

	return 0;
}
