/* 
 * File:   Rectange.cpp
 * Author: uj6
 * 
 * Created on 2017年6月19日, 上午11:05
 */

#include "Rectange.h"

Rectange::Rectange(const Point &bottomLeftPoint, const Point& topRightPoint) {
	m_TopRightX = topRightPoint.getX();
	m_TopRightY = topRightPoint.getY();
	m_BottomLeftX = bottomLeftPoint.getX();
	m_BottomLeftY = bottomLeftPoint.getY();
}

Rectange::Rectange(const Rectange& orig) {
}

Rectange::~Rectange() {
}

Point Rectange::TopRightPoint() const{
	return Point(m_TopRightX, m_TopRightY);
}

Point Rectange::TopLeftPoint() const {
	return Point(m_BottomLeftX, m_TopRightY);
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
