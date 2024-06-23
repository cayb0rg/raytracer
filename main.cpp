#include "common.h"
#include "camera.h"

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
    world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));

    /* Camera */
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.max_depth = 10;
    cam.samples_per_pixel = 10;

    cam.render(world);
}