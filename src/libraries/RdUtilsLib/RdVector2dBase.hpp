// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* *********************************************************************
 * RdWeaponBaseTest
 ***********************************************************************
 *
 * Testing the WeaponBase class
 *
 ***********************************************************************
 */
 
#ifndef __RD_VECTOR_BASE_HPP__
#define __RD_VECTOR_BASE_HPP__

template< class T>
class RdVector2dBase {
    public:
        RdVector2dBase( T x, T y): x(x), y(y) {}
        RdVector2dBase(): x(0), y(0) {}
        T x, y;
};
typedef RdVector2dBase<int> RdVector2d;

#endif // __RD_VECTOR_BASE_HPP__
