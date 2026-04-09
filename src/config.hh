#pragma once
#include "color.hh"

// simulation
inline int WIDTH = 200;
inline int HEIGHT = 100;
inline int CAMERA_ZOOM = 100;
inline int SIZE_SPHERE = 10;
inline int NB_PARTICULES = 1000; 

// ray
inline float kd = 0.9f;
inline float ks = 0.3f;
inline float kr = 0.1f;
inline float ns = 1;
inline int rec_ray = 5;
inline float k_close = 0.001f;
inline float k_far = 800.0f;
inline Color ambient_color(2, 2, 2);
inline Color background_color(30, 30, 30);
inline Color color_light(100,100,100);

// bordure
inline float FLOOR = -200.0f;
inline float ceiling = 200.0f;
inline float left_bordure = -400.0f;
inline float right_bordure = 400.0f;
inline float BACK_BORDURE = 571;
inline float FRONT_BORDURE = 409;

// physics
inline float GRAVITY = 300;

inline float FIXED_DT = 0.004f;
inline int MAX_SUBSTEPS = 200;

inline float MIN_DIST_SPHERE = 1.f;
inline float restitution = 0.1f;
inline float K_PRESSURE = 3.0f;
inline float K_SDF_SMOOTH = 0.0f;
inline float RADIUS_DENSITY = 100.0f;
inline float DENSITY_ID = 0.5f;
