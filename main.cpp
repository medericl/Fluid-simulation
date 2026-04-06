#include "image.hh"
#include "camera.hh"
#include "light.hh"
#include "sphere.hh"
#include "scene.hh"
#include "vector.hh"

#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>
#include <string>


std::vector<Sphere> vector_sphere()
{
    std::vector<Sphere> spheres = {

        Sphere(Point3(-199, 150, 540), 10, Color(0,0,200)), 
        Sphere(Point3(50, 120, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-120, 180, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-199, 100, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-10, 100, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-150, 100, 540), 10, Color(0,0,200)), 
        Sphere(Point3(80, 100, 540), 10, Color(0,0,200)), 
        Sphere(Point3(160, 100, 540), 10, Color(0,0,200)), 
        Sphere(Point3(190, 100, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-80, 140, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-20, 140, 540), 10, Color(0,0,200)), 
        Sphere(Point3(80, 140, 540), 10, Color(0,0,200)), 
        Sphere(Point3(130, 140, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-170, 140, 540), 10, Color(0,0,200)), 
        Sphere(Point3(170, 180, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-30, 190, 540), 10, Color(0,0,200)), 
        Sphere(Point3(0, 196, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-80, -180, 540), 10, Color(0,0,200)), 
        Sphere(Point3(80, -80, 540), 10, Color(0,0,200)), 
        Sphere(Point3(120,-130, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-80, -100, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-120,-150, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-30, -50, 540), 10, Color(0,0,200)), 
        Sphere(Point3(38,20, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-100, -50, 540), 10, Color(0,0,200)), 
        Sphere(Point3(138,20, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-100, 50, 540), 10, Color(0,0,200)), 
        Sphere(Point3(100, 50, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-138,10, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-138,10, 540), 10, Color(0,0,200)), 
        Sphere(Point3(160, 160, 540), 10, Color(0,0,200)), 
        Sphere(Point3(199, 199, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-199, -199, 540), 10, Color(0,0,200)), 
        Sphere(Point3(199, -199, 540), 10, Color(0,0,200)), 
        Sphere(Point3(-199, 199, 540), 10, Color(0,0,200)), 

    };
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

    GLFWwindow* window = glfwCreateWindow(width, height, "Raymarching", nullptr, nullptr);
    if (!window) { glfwTerminate(); return; }
    glfwMakeContextCurrent(window);

    std::vector<unsigned char> pixels(width * height * 3);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    Camera camera(Point3(0,0,0), Point3(0,0,400), Vector3(0,1,0), width, height);
    Scene scene(camera, vector_light(), vector_sphere());

    while (!glfwWindowShouldClose(window)) {
        float x_offset = std::sin(glfwGetTime()) * 300.0f;

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
    Camera camera(Point3(0,0,0), Point3(0,0,400), Vector3(0,1,0), width, height);
    Scene scene(camera, vector_light(), vector_sphere());

    for (int i = 0; i < 2000; i++) {
        scene.update();
    }
}

int main(int argc, char* argv[]) {
    int width = 1000;
    int height = 1000;

    if (argc > 1 && std::string(argv[1]) == "--debug")
        run_debug(width, height);
    else
        run_window(width, height);

    return 0;
}
