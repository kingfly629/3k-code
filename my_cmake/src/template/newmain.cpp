/* 
 * File:   newmain.cpp
 * Author: uj6
 *
 * Created on 2017年6月14日, 下午2:33
 */

#include "templateClass.h"
#include <exception>
#include <iostream>

/*
 * 
 */
int main(int argc, char** argv) {
	try {
		std::cout << "wtk" << std::endl;
		//templateClass<char> *test = new templateClass<char>;
		//delete test;
		//templateClass<char> *test2 = new templateClass<char> (200);
		templateClass<char> test3;
	} catch (std::exception const& ex) {
		std::cerr << "Exception: " << ex.what() << std::endl;
		return -1;
	}
	return 0;
}

