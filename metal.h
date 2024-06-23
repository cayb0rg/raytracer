#ifndef METAL_H
#define METAL_H

#include "common.h"

class metal : public material {
    public:
        metal(const color& a, double f) : albedo(a), fuzz(fuzz < 1 ? fuzz : 1) {}

        virtual bool scatter (
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_in_unit_sphere());
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            // Return true if the scattered ray is not absorbed
            return (dot(scattered.direction(), rec.normal) > 0.0);
        }

    private:
        color albedo;
        double fuzz;
};

#endif