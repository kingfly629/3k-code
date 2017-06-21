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
}
