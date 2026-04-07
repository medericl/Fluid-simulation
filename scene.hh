#ifndef SCENE_HH
#define SCENE_HH

#include "camera.hh"
#include "light.hh"
#include "image.hh"
#include "sphere.hh"

class Scene {
public:

    Camera camera;
    std::vector<Light> list_light;
    std::vector<Sphere> list_sphere;

    Scene(); 
    Scene(Camera Camera, std::vector<Light> list_light, std::vector<Sphere> list_sphere);
    void ray_tracing(Image& image);
    float ray_march(Vector3 ray, Point3 origin);

    float sdf_smooth(Vector3 p);
    float sdf(Vector3 p);
    float sdf_wall(Vector3 p);
    float sdf_diff(Vector3 p);
    Color shade(Point3 origin, Vector3 ray, float t);
    Vector3 normal(Point3 P);
    Color find_color(Point3 inter_point);

    float last_time = 0.0f;

    float calculate_dt();
    void update_density();
    void update_force();
    void update();
    void resolve_collision();

};

#endif
