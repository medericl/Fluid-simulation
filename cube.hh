#pragma once

#include "vector.hh"
#include "color.hh"

class Cube {
public:
    Cube(const Point3& c, float r, const Color& col);

    Point3 center;
    float rayon;
    Color color;

    bool inter(const Point3& origin, const Vector3& direction, double& t);
    float inter_sdf(Point3 p, Point3 center, float rayon);

};