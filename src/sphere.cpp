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
    Vector3 gravity(0, -1, 0);
    gravity = gravity * GRAVITY;

    if (!(center.y - radius <= FLOOR + 0.01f))
        velocity = velocity + (gravity * dt);
}

static float kernel(float radius, float dst)
{
    float val = std::max(0.0f, radius - dst);
    return (val * val * val);
}

void Sphere::update_density(Point3 center, std::vector<Sphere> &list_sphere)
{
    float den = 0;
    for (auto& s: list_sphere)
    {
        if (s.center != center)
        {
            auto dist = (s.center - center).norm();
            den += kernel(RADIUS_DENSITY, dist);
        }
    }

    density = den;
    pressure = K_PRESSURE * (density - 0.5f);
    //pressure = K_PRESSURE * std::max(0.0f, density - DENSITY_ID);
    if (pressure < 0) {
        pressure *= 0.1f;
    }
    std::cout << pressure << "\n";

    color.r = velocity.norm() * 2;
    color.g = center.y / 50;

    //std::cout << "den: " << den << "\n";
    //std::cout << "pres: " << pressure << "\n";
}

void Sphere::calculate_border()
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


    if (center.z - radius < FRONT_BORDURE) {
        center.z = FRONT_BORDURE + radius;
        velocity.z = -velocity.z * restitution;
    }

    if (center.z + radius > BACK_BORDURE) {
        center.z = BACK_BORDURE - radius;
        velocity.z = -velocity.z * restitution;
    }

}

float Sphere::Calculate_dt()
{
    float now = (float)glfwGetTime();
    if (last_time == 0.0f) last_time = now;
    float dt = now - last_time;
    last_time = now;
    if (dt > 0.05f) dt = 0.05f;
    return dt;
}



void Sphere::update_pos(float a)
{

    float dt = a;
    dt = Calculate_dt();
    if (density != 0) {
        //std::cout << velocity << " old\n";
        velocity = velocity + (force / density) * dt;
        //std::cout << velocity << " new\n";
    }

    //calculate_gravity(dt);
    center = center + (velocity * dt);

    calculate_border();

    //velocity = velocity * 0.994f;
}
