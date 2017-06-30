/* 
 * File:   Rectange.h
 * Author: uj6
 *
 * Created on 2017年6月19日, 上午11:05
 */

#ifndef RECTANGE_H
#define	RECTANGE_H

#include <iostream>

class Point {
public:
    Point() {
        std::cout << "Point default construct..." << std::endl;
    }

    Point(float _x, float _y) {
        std::cout << "Point here parameter construct..." << std::endl;
        x = _x;
        y = _y;
    }

    Point(const Point& orig) {
        std::cout << "Point here copy construct..." << std::endl;
        x = orig.x;
        y = orig.y;
    }

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

    void PrintInfo() {
        std::cout << "(" << x << "," << y << ")" << std::endl;
    }
private:
    float x;
    float y;
};

class Rectange {
public:
    Rectange();
    Rectange(const Point &bottomLeftPoint, const Point topRightPoint);
    //Rectange(const Rectange& orig);
    //const Rectange& operator =(const Rectange& orig);
    virtual ~Rectange();
    Point TopRightPoint() const;
    Point TopLeftPoint() const;
    Point BottomRightPoint() const;
    Point BottomLeftPoint() const;
    float Area() const;
private:
    float m_TopRightX;
    float m_TopRightY;
    float m_BottomLeftX;
    float m_BottomLeftY;
};

#endif	/* RECTANGE_H */

