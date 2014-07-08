// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

 
#ifndef __RD_VECTOR_BASE_HPP__
#define __RD_VECTOR_BASE_HPP__

template< class T>
class RdVector2dBase {
    public:
        RdVector2dBase( T x, T y): x(x), y(y) {}
        RdVector2dBase(): x(0), y(0) {}
        T x, y;
        
        bool operator==( const RdVector2dBase<T>& other_vector) { return this->x == other_vector.x
                    && this->y == other_vector.y; }

        RdVector2dBase& operator+=( const RdVector2dBase<T>& a ) { x+=a.x; y+=a.y; return *this;}
};
typedef RdVector2dBase<int> RdVector2d;

#endif // __RD_VECTOR_BASE_HPP__

