/**
 * @file ray.h
 * @brief This file contains the ray class which represents a ray in 3D space.
 * A ray can be thought of as a function p(t) = A + tB, where:
 * A is the origin, B is the direction, and t is a real number.
 * Replacing parameter 't' with a value gives a point in 3D space.
 */

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:

        ray() {}
        ray(const vec3& origin, const vec3& direction) : orig(origin), dir(direction) {}

        // getters return immutable references to the objects
        vec3 origin() const { return orig; }
        vec3 direction() const { return dir; }

        // return point *at* parameter t
        vec3 at(double t) const {
            return orig + t*dir; // A + tB
        }

    private:
        vec3 orig;
        vec3 dir;
};

#endif