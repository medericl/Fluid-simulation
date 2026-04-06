#ifndef SPHERE_HH
#define SPHERE_HH

#include <vector>
#include "vector.hh"
#include "color.hh"

class Sphere {
public:
    Sphere(const Point3& c, float r, const Color& col);

    Point3 center;
    float radius;
    float last_time = 0.0f;
    Color color;
    Vector3 velocity;
    float density;
    float pressure;
    Vector3 force;

    void update_pos();
    void update_density(Point3 center, std::vector<Sphere> &list_sphere);
    void calculate_gravity(float dt);
    void calculate_border(float dt);
    float calculate_dt();
};

#endif
