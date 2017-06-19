/* 
 * File:   Rectange.cpp
 * Author: uj6
 * 
 * Created on 2017年6月19日, 上午11:05
 */

#include "Rectange.h"

Rectange::Rectange() {
	std::cout << "here self-define default construct..." << std::endl;
}

Rectange::Rectange(const Point &bottomLeftPoint,const Point &topRightPoint) {
	std::cout << "here parameter construct..." << std::endl;
	m_TopRightX = topRightPoint.getX();
	m_TopRightY = topRightPoint.getY();
	m_BottomLeftX = bottomLeftPoint.getX();
	m_BottomLeftY = bottomLeftPoint.getY();
}

//Rectange::Rectange(const Rectange& orig) {
//	std::cout << "here copy construct..." << std::endl;
////	this->m_TopRightX = orig.m_TopRightX;
////	this->m_TopRightY = orig.m_TopRightY;
////	this->m_BottomLeftX = orig.m_BottomLeftX;
////	this->m_BottomLeftY = orig.m_BottomLeftY;
//}

//const Rectange& Rectange::operator =(const Rectange& orig) {
//	std::cout << "here equal construct..." << std::endl;
//	this->m_TopRightX = orig.m_TopRightX;
//	this->m_TopRightY = orig.m_TopRightY;
//	this->m_BottomLeftX = orig.m_BottomLeftX;
//	this->m_BottomLeftY = orig.m_BottomLeftY;
//	return *this;
//}

Rectange::~Rectange() {
}

Point Rectange::TopRightPoint() const {
	return Point(m_TopRightX, m_TopRightY);
}

void Rectange::TopLeftPoint() const {
	Point Point2(m_BottomLeftX, m_TopRightY);
}

Point Rectange::BottomRightPoint() const {
	return Point(m_TopRightX, m_BottomLeftY);
}

Point Rectange::BottomLeftPoint() const {
	return Point(m_BottomLeftX, m_BottomLeftY);
}

float Rectange::Area() const {
	return (m_TopRightX - m_BottomLeftX) * (m_TopRightY - m_BottomLeftY);
}
