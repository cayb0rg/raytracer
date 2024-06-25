#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include <thread>
#include <atomic>
#include <memory>
#include <string>

/**
 * Construct and dispatch rays into the world.
 * Use the results of these rays to construct the rendered image.
 */

struct Camera {
    double aspect_ratio;
    int image_width;
    int samples_per_pixel;
    int max_depth;
    double vertical_fov;
    vec3 lookfrom; // Point camera is looking from
    vec3 lookat;   // Point camera is looking at
    vec3 vup;      // Camera-relative "up" direction

    double defocus_angle;  // Variation angle of rays through each pixel
    double focus_dist;     // Distance from camera lookfrom point to plane of perfect focus

    // Previous Private variables
    vec3 center;            // Camera center
    vec3 pixel00_loc;       // Location of the first pixel 0,0
    vec3 pixel_delta_u;     // Offset between pixels horizontally
    vec3 pixel_delta_v;     // Offset between pixels vertically
    int image_height;       // Rendered image height
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples
    vec3 u, v, w;           // Camera frame basis vectors
    vec3 defocus_disk_u;    // Defocus disk horizontal radius
    vec3 defocus_disk_v;    // Defocus disk vertical radius
};

void initialize(Camera &cam) {
    /* Camera and Viewport Setup */
    cam.center = cam.lookfrom;
    auto theta = degrees_to_radians(cam.vertical_fov);
    auto h = tan(theta / 2);
    auto viewport_height = 2 * h * cam.focus_dist;
    auto viewport_width = viewport_height * (double(cam.image_width) / cam.image_height);

    cam.w = unit_vector(cam.lookfrom - cam.lookat); // camera forward
    cam.u = unit_vector(cross_product(cam.vup, cam.w)); // camera right
    cam.v = cross_product(cam.w, cam.u); // camera up

    auto viewport_u = cam.u * viewport_width;
    auto viewport_v = -cam.v * viewport_height; // negative because y increases downwards

    cam.pixel_delta_u = viewport_u / cam.image_width;
    cam.pixel_delta_v = viewport_v / cam.image_height;

    auto viewport_upper_left = cam.center
        - cam.focus_dist * cam.w
        - viewport_u / 2 - viewport_v / 2;
    cam.pixel00_loc = viewport_upper_left + 0.5 * (cam.pixel_delta_u + cam.pixel_delta_v);

    cam.pixel_samples_scale = 1.0 / cam.samples_per_pixel;

    auto defocus_radius = cam.focus_dist * tan(degrees_to_radians(cam.defocus_angle / 2));
    cam.defocus_disk_u = cam.u * defocus_radius;
    cam.defocus_disk_v = cam.v * defocus_radius;
}

vec3 sample_square() {
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

vec3 defocus_disk_sample(const Camera &cam) {
    auto p = random_in_unit_disk();
    return cam.center + (p[0] * cam.defocus_disk_u) + (p[1] * cam.defocus_disk_v);
}

ray get_ray(const Camera &cam, int i, int j) {
    auto offset = sample_square();
    auto pixel_sample = cam.pixel00_loc +
                        ((i + offset.x()) * cam.pixel_delta_u) +
                        ((j + offset.y()) * cam.pixel_delta_v);
    auto ray_origin = (cam.defocus_angle <= 0) ? cam.center : defocus_disk_sample(cam);
    auto ray_direction = pixel_sample - ray_origin;
    return ray(ray_origin, ray_direction);
}

color ray_color(const ray& r, std::shared_ptr<hittable> world, int depth, int max_depth) {
    if (depth >= max_depth) {
        return color(0, 0, 0);
    }

    hit_record rec;
    if ((*world).hit(r, interval(0.001, infinity), rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth + 1, max_depth);
        }
        return color(0.0, 0.0, 0.0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

void render(std::shared_ptr<Camera> cam, const std::shared_ptr<hittable> world, std::string &out, int start_x, int start_y, int end_x, int end_y) {
    initialize(*cam);

    for (int j = start_y; j < end_y; j++) {
        for (int i = start_x; i < end_x; i++) {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < (*cam).samples_per_pixel; sample++) {
                ray r = get_ray(*cam, i, j);
                pixel_color += ray_color(r, world, 0, (*cam).max_depth);
            }

            out += write_color((*cam).pixel_samples_scale * pixel_color);
        }
    }
}

#endif
