#include "common.h"
#include "camera.h"
#include "diffuse.h"
#include "metal.h"
#include "dielectric.h"

/**
 * What does a Raytracer do?
 * 1. Shoot rays from the camera into the scene
 * 2. Determine what the ray hits
 * 3. Determine the color of the hit
 * 4. Write the color to the image
 */
int main() {
    srand(time(NULL));

    /* World Setup */
    hittable_list world;
    auto material_ground = make_shared<diffuse>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<diffuse>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5); // Air to Glass
    auto material_bubble = make_shared<dielectric>(1.00 / 1.5); // Glass to Air
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(vec3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(vec3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(vec3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.add(make_shared<sphere>(vec3( 1.0,    0.0, -1.0),   0.5, material_right));

    /* Camera */
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.max_depth = 10;
    cam.samples_per_pixel = 10;
    cam.vertical_fov = 20.0;
    cam.lookfrom = vec3(-2,2,1);
    cam.lookat = vec3(0,0,-1);
    cam.vup = vec3(0,1,0);

    cam.render(world);
}