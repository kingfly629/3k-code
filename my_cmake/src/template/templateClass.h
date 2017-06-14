/* 
 * File:   templateClass.h
 * Author: uj6
 *
 * Created on 2017年6月14日, 下午2:29
 */

#ifndef TEMPLATECLASS_H
#define	TEMPLATECLASS_H

template<class T> 
class templateClass {
public:
    templateClass();
    templateClass(T k);
    templateClass(const templateClass& orig);
    virtual ~templateClass();
private:
    T m_k;
};

#endif	/* TEMPLATECLASS_H */

