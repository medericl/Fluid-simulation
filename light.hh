#ifndef LIGHT_HH 
#define LIGHT_HH

#include "vector.hh"
#include "color.hh"
#include "config.hh"

class Light {
public:
    Light() : origin(0,100,0), color(color_light)
    { }

    Light(Point3 o)
        : origin(o), color(color_light)
    { }
    
    Point3 origin;
    Color color;
};

#endif
