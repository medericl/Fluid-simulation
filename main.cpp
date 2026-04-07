#include "config.hh"
#include "image.hh"
#include "camera.hh"
#include "light.hh"
#include "sphere.hh"
#include "scene.hh"
#include "vector.hh"

#include <GLFW/glfw3.h>
#include <cmath>
#include <random>
#include <string>


std::vector<Sphere> vector_sphere()
{
    std::vector<Sphere> spheres = {
        Sphere(Point3(-199, 150, 540), SIZE_SPHERE, Color(1,0,200)), 
    };

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist_x(-80, 80);
    std::uniform_int_distribution<int> dist_z(-30, 30);
    for (int i = 0; i < NB_PARTICULES; i++) {
        //spheres.push_back(Sphere(Point3(dist_x(rng), 199, 540 + dist_x(rng)), SIZE_SPHERE, Color(0, 0, 200)));
        spheres.push_back(Sphere(Point3(dist_x(rng), 199, 540), SIZE_SPHERE, Color(0, 0, 200)));
    }

    return spheres;
}

std::vector<Light> vector_light()
{
    std::vector<Light> lights = {
        Light(Point3(200,300,200)),
        Light(Point3(-200,300,200)),
    };
    return lights;
}


static void render_to_pixels(int width, int height, float x_offset, std::vector<unsigned char>& pixels, Scene& scene)
{
    scene.update();
    Image image(width, height);
    scene.ray_tracing(image);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const Color& c = image.getPixel(x, y);
            int idx = ((height - 1 - y) * width + x) * 3; // flip Y for OpenGL
            pixels[idx + 0] = c.r;
            pixels[idx + 1] = c.g; 
            pixels[idx + 2] = c.b;
        }
    }
}

static void run_window(int width, int height)
{
    if (!glfwInit())
        return;

    int win_width = width * 4;
    int win_height = height * 4;
    GLFWwindow* window = glfwCreateWindow(win_width, win_height, "Raymarching", nullptr, nullptr);
    if (!window) { glfwTerminate(); return; }
    glfwMakeContextCurrent(window);

    std::vector<unsigned char> pixels(width * height * 3);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    Camera camera(Point3(0,150,0), Point3(0,121,96), Vector3(0,1,0), width, height);
    Scene scene(camera, vector_light(), vector_sphere());

    bool b_was_pressed = false;
    bool s_was_pressed = false;

    while (!glfwWindowShouldClose(window)) {
        float x_offset = std::sin(glfwGetTime()) * 300.0f;

        bool b_now = glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS;
        if (b_now && !b_was_pressed)
            scene.add_big_sphere();
        b_was_pressed = b_now;

        bool s_now = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
        if (s_now && !s_was_pressed)
            scene.splash();
        s_was_pressed = s_now;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            scene.add_drop(0);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            scene.add_drop(1);

        render_to_pixels(width, height, x_offset, pixels, scene);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-1, -1);
        glTexCoord2f(1, 0); glVertex2f( 1, -1);
        glTexCoord2f(1, 1); glVertex2f( 1,  1);
        glTexCoord2f(0, 1); glVertex2f(-1,  1);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &tex);
    glfwDestroyWindow(window);
    glfwTerminate();
}

static void run_debug(int width, int height)
{
    Camera camera(Point3(0,0,0), Point3(0,0,CAMERA_ZOOM), Vector3(0,1,0), width, height);
    Scene scene(camera, vector_light(), vector_sphere());

    for (int i = 0; i < 2000; i++) {
        scene.update();
    }
}

int main(int argc, char* argv[]) {
    int width = WIDTH;
    int height = HEIGHT;

    if (argc > 1 && std::string(argv[1]) == "--debug")
        run_debug(width, height);
    else
        run_window(width, height);

    return 0;
}
