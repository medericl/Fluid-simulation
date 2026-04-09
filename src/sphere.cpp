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

// smooth
static float poly6(float h, float r2)
{
    float h2 = h * h;
    if (r2 >= h2) return 0.0f;
    float diff = h2 - r2;
    float h6 = h2 * h2 * h2;
    return (diff * diff * diff) / h6;
}

void Sphere::update_density(Point3 center, std::vector<Sphere> &list_sphere)
{
    float den = 0;
    for (auto& s: list_sphere)
    {
        Vector3 diff = s.center - center;
        float r2 = diff.dot(diff);
        den += MASS * poly6(RADIUS_DENSITY, r2);
    }

    density = den;
    pressure = K_PRESSURE * (density - DENSITY_ID);
    // negatif ??

    //color.r = velocity.norm() * 2;
    color.g = den * 180;
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

void Sphere::update_pos(float dt)
{
    if (density > 1e-6f) {
        velocity = velocity + (force / density) * dt;
    }

    calculate_gravity(dt);
    center = center + (velocity * dt);

    calculate_border();
}
