/**
 * This file contains the vec3 class which represents a 3D vector or point.
 */

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
    public:
        double e[3]; // using double instead of float because it is more accurate
        // float is 32-bit, double is 64-bit

        vec3() : e{0, 0, 0} {} // default constructor
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {} // constructor

        double x() const { return e[0]; } // getter for x
        double y() const { return e[1]; } // getter for y
        double z() const { return e[2]; } // getter for z

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } // unary minus operation

        double operator[](int i) const { return e[i]; } // getter for element i
        double& operator[](int i) { return e[i]; } // setter for element i

        // operator overloading for vector addition
        vec3& operator+=(const vec3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];

            return *this; // this is a pointer to the object that called the function
        }

        // operator overloading for multiplication by scalar
        vec3& operator*=(double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;

            return *this;
        }

        // operator overloading for division by scalar
        vec3& operator/=(double t) {
            return *this *= 1/t; // this is calling the overloaded multiplication operator above!
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        double length() const { // the const keyword means that the function does not modify the object
            return std::sqrt(length_squared());
        }

        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

        // Returns true if the vector is close to zero in all dimensions
        bool near_zero() const {
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }
};

// Vector utility functions
// 'inline' keyword tells the compiler to replace the function call with the function body
// the benefit of this is that it can improve performance by reducing the overhead of function calls

// Print vector to output stream
// example usage: std::cout << vec3(1, 2, 3);
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Vector addition
// example usage: vec3 u(1, 2, 3); vec3 v(4, 5, 6); vec3 w = u + v;
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Vector subtraction
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Vector multiplication by scalar
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v; // this is calling the overloaded multiplication operator above!
}

// Vector multiplication
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0]*v.e[0], u.e[1]*v.e[1], u.e[2]*v.e[2]);
}

// Vector division by scalar
inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v; // this is calling the overloaded multiplication operator above!
}

// Vector dot product
// Dot product is useful for determining how much one vector points in the direction of another
// or how similar/dissimilar they are
// It returns 0 for perpendicular vectors, 1 for parallel vectors pointing in the same direction
// and -1 for parallel vectors pointing in opposite directions
inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2];
}

// Vector cross product
// It returns a vector that is perpendicular to both u and v, w
// . . . w . . .
// . . . | . . .
// . . . | . . .
// . . . o . . .
// . . . | \ . .
// . . . | .\. .
// . . . | . \ .
// . . . v . .u.
// w is 0 in length when u and v are parallel
// w is 1 in length when u and v are perpendicular

// | i  j  k |
// | u1 u2 u3| = i | u2 u3 | - j | u1 u3 | + k | u1 u2 |
// | v1 v2 v3|     | v2 v3 |     | v1 v3 |     | v1 v2 |
//                u2v3 - u3v2   u3v1 - u1v3   u1v2 - u2v1
inline vec3 cross_product(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1]*v.e[2] - u.e[2]*v.e[1],  // i
                u.e[2]*v.e[0] - u.e[0]*v.e[2],  // j
                u.e[0]*v.e[1] - u.e[1]*v.e[0]); // k
}

// Returns a vector that points in the same direction as v with length 1
inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

// Returns a vector inside the unit sphere
inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

// Returns a vector on the surface of the unit sphere
inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) {
        // in the same hemisphere as the normal
        return on_unit_sphere;
    } else {
        // in the opposite hemisphere as the normal
        return -on_unit_sphere;
    }
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    double length = dot(v, n);
    return v - 2*length*n;
}

#endif