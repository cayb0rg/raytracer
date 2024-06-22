#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "common.h"
#include <memory>
#include <vector>

class hittable_list : public hittable {
    public:
        std::vector<std::shared_ptr<hittable>> objects;
        // shared_ptr<type> is a smart pointer with reference-counting semantics
        // when shared pointers go out of scope, the reference count is decremented
        // when the reference count reaches zero, the object is deleted

        // a shared pointer is initialized like:
        // std::shared_ptr<type> object = std::make_shared<type>(args);

        // vector is a dynamic array that can grow and shrink in size

        hittable_list() {}
        hittable_list(std::shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }

        void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            for (const auto& object : objects) {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t; // update the closest hit
                    rec = temp_rec; // copy the hit record
                }
            }

            return hit_anything;
        }
};

#endif