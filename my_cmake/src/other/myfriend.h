/* 
 * File:   myfriend.h
 * Author: uj6
 *
 * Created on 2017年6月19日, 下午3:36
 */

#ifndef MYFRIEND_H
#define	MYFRIEND_H
#include <iostream>

namespace kkk {

    class myfriend {
    public:
        myfriend(int a = 1);
        myfriend(const myfriend& orig);
        myfriend& operator=(const myfriend& orig);
        friend void friend_PrintInfo(const myfriend& obj);
        friend class classB;
        virtual ~myfriend();
        void test();
    private:
        int x;
    };

    class classB {
    public:
        classB(myfriend &_a);
        classB(const classB& orig);
        virtual ~classB();
        void test();
    private:
        myfriend a;
        int x;
    };

    class CFather {
    public:
        CFather();
        CFather(int _a);
        virtual ~CFather();
        virtual void test();
    protected:
        int a;
    };

    class CDerived : public CFather {
    public:
        CDerived();
        CDerived(int _b);
        ~CDerived();
        void test();
    private:
        int b;
    };

}
#endif	/* MYFRIEND_H */

