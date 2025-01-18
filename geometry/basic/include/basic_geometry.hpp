#pragma once

#include <complex>
#include <utility>
#include <cmath>
#include <stdexcept>


using std::pair;
using std::make_pair;
using std::swap;


template <typename T>
class vec2d {
public:
    T x, y;

    vec2d(T x = 0, T y = 0) : x(x), y(y) {}

    // Array subscript operator
    T& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;
        throw std::out_of_range("Index out of range");
    }

    const T& operator[](int index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        throw std::out_of_range("Index out of range");
    }

    // Addition
    vec2d<T> operator+(const vec2d<T>& other) const {
        return vec2d<T>(x + other.x, y + other.y);
    }

    // Subtraction
    vec2d<T> operator-(const vec2d<T>& other) const {
        return vec2d<T>(x - other.x, y - other.y);
    }

    // Scalar multiplication
    vec2d<T> operator*(T scalar) const {
        return vec2d<T>(x * scalar, y * scalar);
    }

    // Dot product
    T dot(const vec2d<T>& other) const {
        return x * other.x + y * other.y;
    }

    // Magnitude
    T magnitude() const {
        return std::sqrt(x*x + y*y);
    }

    // Normalization
    vec2d<T> normalize() const {
        T mag = magnitude();
        if (mag == 0) throw std::domain_error("Cannot normalize zero vector");
        return vec2d<T>(x / mag, y / mag);
    }
};

// Scalar multiplication (scalar * vector)
template <typename T>
vec2d<T> operator*(T scalar, const vec2d<T>& vec) {
    return vec * scalar;
}


//Find the center of the circumscribed circle as the point of intersection of the
//perpendicular bisectors of the sides of the triangle

template<
    typename TCoord,
    typename TVector
>
pair<TVector, TCoord> circumscribed_circle_2d(
    const TVector &v1,
    const TVector &v2,
    const TVector &v3)
{
    TVector v_center_12 = (v1 + v2) * (1.0/2.0);
    TVector v_center_13 = (v1 + v3) * (1.0/2.0);

    TVector n_12 = (v2 - v1);

    TVector n_13 = (v3 - v1);

    // System of linear EQs
    const TCoord& a00 = n_12[0], a01 = n_12[1],
        a10 = n_13[0], a11 = n_13[1],
        b0 = n_12[0]*v_center_12[0] + n_12[1]*v_center_12[1],
        b1 = n_13[0]*v_center_13[0] + n_13[1]*v_center_13[1];

    const TCoord det = a00*a11 - a01*a10;
    const TCoord det_x = b0*a11 - a01*b1;
    const TCoord det_y = a00*b1 - b0*a10;
    
    TCoord x = det_x / det;
    TCoord y = det_y / det;
 
    TVector circumscribed_circle_center = TVector(x, y);
    TCoord dx = v1[0] - x;
    TCoord dy = v1[1] - y;
                                            
    TCoord r_sq = dx*dx + dy*dy;

    return make_pair(circumscribed_circle_center, r_sq);
}

