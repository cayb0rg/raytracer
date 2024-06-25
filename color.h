/**
 * @brief Write the color value to the output stream
 */

#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
#include <iostream>
#include <string>

// C++ NOTE:
// difference between "using color = vec3;" vs "class color : public vec3 {};"
// the former is a type alias, the latter is a derived class
// the derived class is more flexible because it can have additional members and methods
// the derived class can be used in place of vec3, but not vice versa (without a cast)
// the derived class can be passed to functions that take vec3, but not vice versa (without a cast)

// class color : public vec3 {
//     public:
//         color() : vec3() {}
//         color(double r, double g, double b) : vec3(r, g, b) {}

//         double r() const { return e[0]; }
//         double g() const { return e[1]; }
//         double b() const { return e[2]; }
// };

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return sqrt(linear_component);

    return 0;
}

using color = vec3;

std::string write_color(color pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate  the [0,1] value of each color component to [0,255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(intensity.clamp(r) * 256);
    int gbyte = int(intensity.clamp(g) * 256);
    int bbyte = int(intensity.clamp(b) * 256);

    // Write out the pixel color components
    std::string out = std::to_string(rbyte) + " " + std::to_string(gbyte) + " " + std::to_string(bbyte) + "\n";
    return out;
}

#endif