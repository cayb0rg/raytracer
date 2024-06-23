#include "common.h"
#include "camera.h"
#include "diffuse.h"
#include "metal.h"

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
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(vec3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(vec3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(vec3( 1.0,    0.0, -1.0),   0.5, material_right));


    /* Camera */
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.max_depth = 10;
    cam.samples_per_pixel = 10;

    cam.render(world);
}