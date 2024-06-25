#include "common.h"
#include "camera.h"
#include <thread>
#include <string>

/**
 * What does a Raytracer do?
 * 1. Shoot rays from the cam into the scene
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

    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.max_depth = 10;
    cam.samples_per_pixel = 10;
    cam.vertical_fov = 20.0;
    cam.lookfrom = vec3(-2,2,1);
    cam.lookat = vec3(0,0,-1);
    cam.vup = vec3(0,1,0);
    cam.defocus_angle = 10.0;
    cam.focus_dist = 3.4;
    initialize(cam);
    auto cam_ptr = std::make_shared<Camera>(cam);

    // Initialize threads
    std::thread thread_pool[4];
    std::string string_ptrs[4];
    for (int i = 0; i < 4; i++) {
        string_ptrs[i] = std::string();
    }

    /* Image Setup */
    int aspect_ratio = 16.0 / 9.0;
    int image_width = 800;
    int image_height = (int)(image_width / aspect_ratio);
    // Make sure image_height is greater than or equal to 1
    image_height = image_height < 1 ? 1 : image_height;

    /* Image Output*/
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    // Pixels are written out in rows, top to bottom
    // Split the img into four and give each part to a thread
    int rows_per_thread = image_height / 2;
    int cols_per_thread = image_width / 2;
    int thread_counter = 0;
    auto world_ptr = std::make_shared<hittable_list>(world); // If moving `world` to a shared_ptr is acceptable
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 2; i++) {
            int start_x = i * rows_per_thread;
            int start_y = j * cols_per_thread;
            int end_x = start_x + rows_per_thread;
            int end_y = start_y + cols_per_thread;

            std::string &ppm_single = string_ptrs[thread_counter];

            std::clog << "Initiating " << thread_counter << " thread in range [" << start_x << ", " << end_x << "), [" << start_y << ", " << end_y << ")\n";

            thread_pool[thread_counter++] = std::thread([&ppm_single, cam_ptr, world_ptr, start_x, start_y, end_x, end_y]() {
                render(cam_ptr, world_ptr, ppm_single, start_x, start_y, end_x, end_y);
            });
        }
    }
    std::clog << "Threads initiated.\n";

    // Wait for thread t1 to finish
    for (int i = 0; i < 4; i++) {
        thread_pool[i].join();
        std::clog << "Thread " << i << " finished.\n";
    }

    // Loop through all strings and send them to std::cout
    for (int i = 0; i < 4; i++) {
        std::cout << string_ptrs[i];
    }

    std::clog << "\nDone.\n";
}