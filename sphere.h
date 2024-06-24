#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "diffuse.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(vec3 center, double radius, shared_ptr<material> mat) : center(center), radius(fmax(0, radius)), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            // auto a = dot(r.direction(), r.direction());
            auto a = r.direction().length_squared();
            // auto b = -2.0 * dot(r.direction(), oc);
            auto h = dot(r.direction(), oc);
            auto c = dot(oc, oc) - radius * radius;
            // auto discriminant = b*b - 4*a*c;
            auto discriminant = h*h - a*c;

            if (discriminant < 0) {
                return false;
            } else {
                // return the solution to the quadratic equation
                // which represents the intersection point of the ray with the sphere
                // return (-b - sqrt(discriminant)) / (2.0 * a);
                auto root = (h - sqrt(discriminant)) / a;

                // if (root <= t_min || root >= t_max) {
                if (!ray_t.surrounds(root)) {
                    // try the other root
                    root = (h + sqrt(discriminant)) / a;

                    // if (root <= t_min || root >= t_max) {
                    if (!ray_t.surrounds(root)) {
                        return false;
                    }
                }

                vec3 outward_normal = unit_vector(r.at(root) - center);
                vec3 normal;
                bool front_face = false;

                // determine normal vector direction
                if (dot(r.direction(), outward_normal) > 0.0) {
                    // ray is inside the sphere
                    normal = -outward_normal;
                    front_face = false;
                } else {
                    // ray is outside the sphere
                    normal = outward_normal;
                    front_face = true;
                }

                rec.t = root;
                rec.normal = normal;
                rec.p = r.at(rec.t);
                rec.front_face = front_face;
                rec.mat = mat;

                return true;
            }
        }

    private:
        vec3 center;
        double radius;
        shared_ptr<material> mat;
};

#endif