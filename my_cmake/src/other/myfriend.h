/* 
 * File:   myfriend.h
 * Author: uj6
 *
 * Created on 2017年6月19日, 下午3:36
 */

#ifndef MYFRIEND_H
#define	MYFRIEND_H
#include <iostream>

class myfriend {
public:
    myfriend(int a);
    myfriend(const myfriend& orig);
    friend void PrintInfo(const myfriend& obj);
    virtual ~myfriend();
private:
    int x;
};


#endif	/* MYFRIEND_H */

