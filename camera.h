#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

/**
 * Construct and dispatch rays into the world.
 * Use the results of these rays to construct the rendered image.
 */

class camera {
    public:
        double aspect_ratio;
        int image_width;
        int samples_per_pixel;
        int max_depth;
        double vertical_fov = 90;
        vec3 lookfrom = vec3(0,0,0);        // Point camera is looking from
        vec3 lookat   = vec3(0,0,-1);       // Point camera is looking at
        vec3 vup      = vec3(0,1,0);        // Camera-relative "up" direction

        double defocus_angle = 0.0;  // Variation angle of rays through each pixel
        double focus_dist = 10.0;    // Distance from camera lookfrom point to plane of perfect focus

        camera() {}

        void render(const hittable& world) {
            initialize();

            /* Image Output*/
            std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

            // Pixels are written out in rows, top to bottom
            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << image_height - j << " " << std::flush;
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, world);
                    }

                    // Hello, pixel!
                    // auto pixel_color = color(
                    //     double(i) / (image_width - 1), // top right will be red
                    //     double(j) / (image_height - 1),  // bottom left will be green
                    //     0.25
                    // );

                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }
            std::clog << "\nDone.\n";
        }

    private:
        vec3 center; // Camera center
        vec3 pixel00_loc; // Location of the first pixel 0,0
        vec3 pixel_delta_u; // Offset between pixels horizontally
        vec3 pixel_delta_v; // Offset between pixels vertically
        int image_height; // Rendered image height
        double pixel_samples_scale; // Color scale factor for a sum of pixel samples
        vec3   u, v, w;              // Camera frame basis vectors
        vec3 defocus_disk_u; // Defocus disk horizontal radius
        vec3 defocus_disk_v; // Defocus disk vertical radius


        void initialize() {
            /* Image Setup */
            image_height = (int)(image_width / aspect_ratio);
            // Make sure image_height is greater than or equal to 1
            image_height = image_height < 1 ? 1 : image_height;

            /* Camera and Viewport Setup */
            // The camera is the eye through which both we and the raytracer see the world
            // The vector from the camera to the viewport is orthogonal to the viewport
            // The distance between the camera and the viewport is the focal length
            center = lookfrom;
            // auto focal_length = (lookfrom - lookat).length();
            auto theta = degrees_to_radians(vertical_fov);
            auto h = tan(theta / 2);
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = viewport_height * (double(image_width)/image_height);
            // don't use aspect ratio because that is the _ideal_ aspect ratio

            // y-axis is up, x-axis is right, z-axis is forward (right-handed coordinates)
            //|    y+
            //|    |  x+
            //|    | /
            //|    |/
            //|z-  *----> z+
            //^--viewport
            w = unit_vector(lookfrom - lookat); // camera forward
            u = unit_vector(cross_product(vup, w)); // camera right
            v = cross_product(w, u); // camera up


            // If we want the zeroeth pixel in the top-left and work to bottom-right,
            // Then y should increase from top to bottom
            // viewport_u is the horizontal vector, viewport_v is the vertical vector
            auto viewport_u = u * viewport_width;
            auto viewport_v = -v * viewport_height; // negative because y increases downwards

            // Horizonal and vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            std::clog << "Pixel u\n" << pixel_delta_u << "\n Pixel v " << pixel_delta_v << "\n";

            auto viewport_upper_left = center
                - focus_dist * w // cross the focal length to get to the viewport
                - viewport_u/2 - viewport_v/2; // we're at the center of the viewport already, so move to the upper left
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); // center of the first pixel

            pixel_samples_scale = 1.0 / samples_per_pixel;


            // Calculate the camera defocus disk basis vectors.
            auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;

        }

        ray get_ray(int i, int j) {
            // Construct a camera ray originating from the defocus disk and directed at a randomly
            // sampled point around the pixel location i, j.
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc +
                                ((i + offset.x()) * pixel_delta_u) +
                                ((j + offset.y()) * pixel_delta_v);
            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;
            ray r(ray_origin, ray_direction);

            return r;
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        vec3 defocus_disk_sample() const {
            // Returns a random point in the camera defocus disk.
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        color ray_color(const ray& r, const hittable& world, int depth = 0) {
            if (depth >= max_depth) {
                return color(0, 0, 0);
            }

            hit_record rec;
            if (world.hit(r, interval(0.001, infinity), rec)) { // 0.001 to avoid self-intersection
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    // Each ray loses 50% of its color when it bounces
                    return attenuation * ray_color(scattered, world, depth + 1);
                }
                return color(0.0, 0.0, 0.0);
            }

            /* Simple Gradient */
            // when a = 0.0, return white
            // when a = 1.0, return blue
            // blended_value = (1.0 - a) * start_value + a * end_value;
            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0); // scale y to [0,1]
            // lerp the color based on the y-component of the ray direction
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }

};

#endif