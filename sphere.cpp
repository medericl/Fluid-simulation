#include "sphere.hh"
#include "vector.hh"
#include "color.hh"
#include "config.hh"

#include <GLFW/glfw3.h>
#include <iostream>

Sphere::Sphere(const Point3& c, float r, const Color& col)
    : center(c), radius(r), color(col), density(0.0f), pressure(0.0f)
{ }

void Sphere::calculate_gravity(float dt)
{
    Vector3 gravity(0, -100, 0);

    if (!(center.y - radius <= FLOOR + 0.01f))
        velocity = (gravity * dt) + velocity;

    center = (velocity * dt) + center;
}

static float kernel(float radius, float dst)
{
    float val = std::max(0.0f, radius * radius - dst * dst) / 10000.0f;
    return val * val * val;
}

void Sphere::update_density(Point3 center, std::vector<Sphere> &list_sphere)
{
    float den = 0;
    for (auto& s: list_sphere)
    {
        if (s.center != center)
        {
            auto dist = (s.center - center).norm();
            den += kernel(100.0f, dist);
        }
    }

    density = den;
    pressure = 50.0f * (density - 0.6);
    color.r = den * 100.0f;
}

void Sphere::calculate_border(float dt)
{

    if (center.y - radius < FLOOR) {
        center.y = FLOOR + radius;
        velocity.y = -velocity.y * restitution;
    }

    if (center.y + radius > ceiling) {
        center.y = ceiling - radius;
        velocity.y = -velocity.y * restitution;
    }

    if (center.x - radius < left_bordure) {
        center.x = left_bordure + radius;
        velocity.x = -velocity.x * restitution;
    }

    if (center.x + radius > right_bordure) {
        center.x = right_bordure - radius;
        velocity.x = -velocity.x * restitution;
    }

}

float Sphere::calculate_dt()
{
    float now = (float)glfwGetTime();
    if (last_time == 0.0f) last_time = now;
    float dt = now - last_time;
    last_time = now;
    if (dt > 0.05f) dt = 0.05f;
    return dt;
}


void Sphere::update_pos()
{
    float dt = calculate_dt();

    if (density != 0) {
        velocity = velocity + (force / density) * dt;
        center = (velocity * dt) + center;
    }

    //calculate_gravity(dt);
    calculate_border(dt);
}
