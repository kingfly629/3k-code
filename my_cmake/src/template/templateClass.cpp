/* 
 * File:   templateClass.cpp
 * Author: uj6
 * 
 * Created on 2017年6月14日, 下午2:29
 */

#include "templateClass.h"
#include <iostream>

template<class T>
templateClass<T>::templateClass() {
	this->m_k = 100;
	std::cout << "here Construction k=" << this->m_k << std::endl;
}

template<class T>
templateClass<T>::templateClass(T k) {
	this->m_k = k;
	std::cout << "here Parameter Construction k=" << this->m_k << std::endl;
}

template<class T>
templateClass<T>::templateClass(const templateClass& orig) {
	std::cout << "here Copy Construction" << this->m_k << std::endl;
}

template<class T>
templateClass<T>::~templateClass() {
	std::cout << "here Destruction" << std::endl;
}

