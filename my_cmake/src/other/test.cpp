#include "Rectange.h"
#include "myfriend.h"
#include <memory>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <exception>
#include <algorithm>
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
	std::cout << "int=" << c1 << std::endl;
	std::cout << "(int)a/b=" << (int) a / b << std::endl;
	std::cout << "float=" << c2 << std::endl;
	std::cout << "(float)a/b=" << (float) a / b << std::endl;
	std::cout << "double=" << c3 << std::endl;
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

char* str_replace(const char *src, const char *sub, const char* replace) {
	char *result = (char *) malloc(sizeof (char) * (strlen(src) - strlen(sub) + strlen(replace) + 1));
	memset(result, 0x0, sizeof (result));
	std::cout << "sizeof(result)" << sizeof (result) << std::endl;
	char *temp = const_cast<char *> (strstr(src, sub));
	if (!temp) {
		return NULL;
	}
	std::cout << "sizeof(temp)" << temp - src << std::endl;
	//strncpy(result, src, temp - src);
	//result[temp-src] = '\0';
	strncat(result, src, temp - src);
	std::cout << "cur 1:" << strlen(result) << std::endl;
	char *q = strcat(result, replace);
	std::cout << "cur 2:" << strlen(result) << std::endl;
	char *p = strcat(result, temp + strlen(sub));
	std::cout << "cur 3:" << strlen(result) << std::endl;
	//result[strlen(result)] = '\0';(可以不需要，因为strncat会在尾部追加了)

	return p;
}

//bubble sort

void bubble_sort(int []) {

}

//qsort
static int qsort_cmp_func_asc(const void* p, const void* q) {
	return *((int *) p) > *((int *) q);
}

static int qsort_cmp_func_desc(const void* p, const void* q) {
	return *((int *) p) < *((int *) q);
}

static bool sort_cmp_func_desc(const void* p, const void* q) {
	return *((int *) p) < *((int *) q);
}
//quick sort
static int quick_sort(const void* p, const void* q) {
	return *((int *) p) > *((int *) q);
}

int main(void) {
	try {
		std::cout << "=========construction=============" << std::endl;
		//1- construction
		Point a(1.0, 2.0);
		Point b(3.0, 4.1);
		Rectange myRect_tmp(a, b);
		Rectange myRect(myRect_tmp);
		myRect = myRect_tmp;

		Point temp = myRect.TopRightPoint();
		temp.PrintInfo();
		temp = myRect.TopLeftPoint();
		temp.PrintInfo();
		temp = myRect.BottomRightPoint();
		temp.PrintInfo();
		temp = myRect.BottomLeftPoint();
		temp.PrintInfo();
		std::cout << "Area = " << myRect.Area() << std::endl;

		std::cout << "=========friend function=============" << std::endl;

		//2- friend function
		myfriend obj(3);
		friend_PrintInfo(obj);

		std::cout << "==========friend class============" << std::endl;

		//3- friend class
		//myfriend obj(3);
		classB bb(obj);
		bb.test();

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

		std::cout << "==========str_replace============" << std::endl;
		const char *src = "wangjing jiang";
		const char *sub = "jing";
		const char *replace = "jin";
		std::cout << "before replace:" << src << std::endl;
		char *p = str_replace(src, sub, replace);
		std::cout << "after replace:" << p << std::endl;
		free(p);

		std::cout << "==========strcpy============" << std::endl;
		char *dest = new char [strlen(src) + 1];
		memset(dest, 0x0, strlen(src) + 1);
		std::cout << "src:" << src << "; strlen(src)" << strlen(src) << std::endl;
		my_strcpy(dest, src);
		std::cout << "dest:" << dest << std::endl;
		delete [] dest;

		std::cout << "==========typedef function & callback============" << std::endl;
		//CallBack func = division;
		//func(1,3);
		test(3, 2, division);

		std::cout << "==========force type transfer============" << std::endl;
		int i = 17;
		char j = 'c'; /* ascii 值是 99 */
		float sum1 = (float) i + j;
		float sum2 = i + j;
		//printf("Value of sum : %f,%f\n", sum1, sum2);
		std::cout << "Value of sum1 :" << sum1 << "; Value of sum2 :" << sum2 << std::endl;

		std::cout << "==========derived class============" << std::endl;
		//CDerived child;
		//child.test();
		CFather *father = new CDerived(100);
		father->test();
		delete father;

		std::cout << "========== sort ============" << std::endl;
		int test[] = {3, 2, 6, 2, 7, 9, 0, 8};
		printf("before :\n");
		for (int k = 0; k < 8; ++k) {
			printf("test[%d] = %d\n", k, test[k]);
		}

		//qsort cmp_func asc
		qsort(test, 8, sizeof (int), qsort_cmp_func_asc);
		printf("after qsort :\n");
		for (int k = 0; k < 8; ++k) {
			printf("test[%d] = %d\n", k, test[k]);
		}

		//qsort cmp_func desc
		int test2[] = {3, 2, 6, 2, 7, 9, 0, 8};
		qsort(test2, 8, sizeof (int), qsort_cmp_func_desc);
		printf("after qsort with cmp_func:\n");
		for (int k = 0; k < 8; ++k) {
			printf("test2[%d] = %d\n", k, test2[k]);
		}

		//sort c++ desc
		int test3[] = {3, 2, 6, 2, 7, 9, 0, 8};
		sort(test3, test3 + 8, sort_cmp_func_desc);
		printf("after sort with cmp_func:\n");
		for (int k = 0; k < 8; ++k) {
			printf("test3[%d] = %d\n", k, test3[k]);
		}
	} catch (const std::exception &ex) {
		std::cerr << "catch Exception: " << ex.what() << std::endl;
		fprintf(stderr, "catch Exception : %s", ex.what());
	}
	return 0;
}
