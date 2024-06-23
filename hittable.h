#ifndef HITTABLE_H
#define HITTABLE_H

#include "common.h"

class hit_record {
    public:
        vec3 p;
        vec3 normal;
        double t;
        bool front_face;
        shared_ptr<material> mat;

};

class hittable {
    public:
        virtual ~hittable() {}

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif