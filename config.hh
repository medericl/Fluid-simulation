#pragma once
#include "color.hh"

inline float kd = 0.9f;
inline float ks = 0.3f;
inline float kr = 0.1f;
inline float ns = 1;
inline int rec_ray = 5;
inline float k_close = 0.001f;
inline float k_far = 1000.0f;
inline Color ambient_color(2, 2, 2);
inline Color background_color(30, 30, 30);
inline Color color_light(100,100,100);

inline float restitution = 0.1f;
inline float FLOOR = -200.0f;
inline float ceiling = 200.0f;
inline float left_bordure = -200.0f;
inline float right_bordure = 200.0f;
