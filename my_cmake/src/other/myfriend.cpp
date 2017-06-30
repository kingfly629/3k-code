/* 
 * File:   myfriend.cpp
 * Author: uj6
 * 
 * Created on 2017年6月19日, 下午3:36
 */

#include "myfriend.h"

namespace kkk {

	myfriend::myfriend(int a) {
		std::cout << "myfriend here parameter construct..." << std::endl;
		x = a;
	}

	myfriend::myfriend(const myfriend& orig) {
		std::cout << "myfriend here copy construct..." << std::endl;
		this->x = orig.x;
	}

	myfriend& myfriend::operator=(const myfriend& orig) {
		std::cout << "myfriend here equal construct..." << std::endl;
		this->x = orig.x;
		return *this;
	}

	myfriend::~myfriend() {
	}

	void myfriend::test() {
		std::cout << "here myfriend test()..." << std::endl;
	}

	classB::classB(myfriend &_a) : a(_a) {
	}

	classB::classB(const classB& orig) {
	}

	classB::~classB() {
	}

	void classB::test() {
		std::cout << "here friend class:" << a.x << std::endl;
	}

	CFather::CFather() {
		a = 1;
		std::cout << "here Father Default Constrution:" << a << std::endl;
	}

	CFather::CFather(int _a) : a(_a) {
		std::cout << "here Father Constrution:" << a << std::endl;
	}

	CFather::~CFather() {
		std::cout << "here Father Destrution:" << std::endl;
	}

	void CFather::test() {
		std::cout << "Father a:" << a << std::endl;
	}

	CDerived::CDerived() {
		//a = 1;
		b = 2;
		std::cout << "here Child Default Constrution:" << a << "," << b << std::endl;
	}

	CDerived::CDerived(int _a) : CFather(_a) {
		b = 200;
		std::cout << "here Child Parameter Constrution:" << a << "," << b << std::endl;
	}

	CDerived::~CDerived() {
		std::cout << "here Child Destrution:" << std::endl;
	}

	void CDerived::test() {
		std::cout << "Derived a:" << a << std::endl;
	}
}
