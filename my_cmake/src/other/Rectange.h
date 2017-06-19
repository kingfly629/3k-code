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

    Point(float _x, float _y) {
        x = _x;
        y = _y;
    }
    Point(const Point& orig);

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
    Rectange(const Point &bottomLeftPoint, const Point& topRightPoint);
    Rectange(const Rectange& orig);
    Point TopRightPoint() const;
    Point TopLeftPoint() const;
    Point BottomRightPoint() const;
    Point BottomLeftPoint() const;
    float Area() const;
    virtual ~Rectange();
private:
    float m_TopRightX;
    float m_TopRightY;
    float m_BottomLeftX;
    float m_BottomLeftY;
};

#endif	/* RECTANGE_H */

