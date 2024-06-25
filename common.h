#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>
#include <time.h>


// C++ Std Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Returns a number in the range [0, 1)
inline double random_double() {
    return std::rand() / (RAND_MAX + 1.0);
}

// Returns a number in the given range
inline double random_double(int min, int max) {
    return min + (max-min)*random_double();
}

// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include "material.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "diffuse.h"
#include "metal.h"
#include "dielectric.h"

#endif