#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

class hit_record;

class material {
    public:
        virtual ~material() {} // Destructor

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
        ) const {
            return false;
        }
};

#endif