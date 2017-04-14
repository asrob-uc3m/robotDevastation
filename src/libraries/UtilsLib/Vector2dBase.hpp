// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

 
#ifndef __RD_VECTOR_BASE_HPP__
#define __RD_VECTOR_BASE_HPP__

namespace rd{

/**
 * @ingroup UtilsLib
 *
 * @brief Class representing a 2D vector
 *
 * Includes basic functions for comparison and basic operations
 */

template< class T>
class Vector2dBase {
    public:
        Vector2dBase( T x, T y): x(x), y(y) {}
        Vector2dBase(): x(0), y(0) {}
        
        bool operator==( const Vector2dBase<T>& other_vector )
        {
            return x == other_vector.x && y == other_vector.y;
        }

        Vector2dBase& operator+=( const Vector2dBase<T>&a )
        {
            x += a.x;
            y += a.y;
            return *this;
        }

        const T & getX() const
        {
            return x;
        }

        const T & getY() const
        {
            return y;
        }

    private:
        T x, y;
};

//! @brief Default 2D vector
typedef Vector2dBase<int> Vector2d;

}

#endif // __RD_VECTOR_BASE_HPP__

