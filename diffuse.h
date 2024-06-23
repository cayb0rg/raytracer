#ifndef DIFFUSE_H
#define DIFFUSE_H

#include "common.h"
#include "material.h"

class diffuse : public material {
    public:
        diffuse(const color& a) : albedo(a) {}

        virtual bool scatter (
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 direction = rec.normal + random_unit_vector();

            if (direction.near_zero()) {
                direction = rec.normal;
            }

            scattered = ray(rec.p, direction);
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
};

#endif