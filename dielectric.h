#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "common.h"

class dielectric : public material {
    public:
        dielectric(double refractive_index) : refractive_index(refractive_index) {}

        virtual bool scatter (
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0 / refractive_index) : refractive_index;

            vec3 unit_direction = unit_vector(r_in.direction());

            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;

            vec3 direction;

            // Schlick's approximation
            if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, ri);
            }

            scattered = ray(rec.p, direction);
            return true;
        }

    private:
        // Ratio of the material's refractive index over
        // the refractive index of the enclosing media
        double refractive_index;

        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance
            double r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
};

#endif