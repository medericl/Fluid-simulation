#include "cube.hh"
#include "vector.hh"
#include "color.hh"


Cube::Cube(const Point3& c, float r, const Color& col)
    : center(c), rayon(r), color(col)
{ }

bool Cube::inter(const Point3& origin, const Vector3& direction, double& t) {
    return 0;
}

float Cube::inter_sdf(Point3 p, Point3 center, float rayon) {
    return 0;
}