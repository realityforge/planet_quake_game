////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __OBJFUNCTOR_H__
#define __OBJFUNCTOR_H__

#include "Functor.h"

// class: ObjFunctor
template<class T>
class ObjFunctor : public Functor
{
public:
	typedef int (T::*funcType)();
    ObjFunctor(T *o, funcType f) 
    { 
        obj = o; 
        m_Function = f;
    }
    int operator ()()
    {
        return (obj->*m_Function)(); 
    }
protected:
	T *obj;
	
	funcType m_Function;
};

#endif
