#include "scene.hh"
#include "color.hh"
#include "sphere.hh"
#include "vector.hh"
#include "config.hh"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

Scene::Scene(Camera Camera, std::vector<Light> List_light, std::vector<Sphere> List_sphere)
: camera(Camera), list_light(List_light), list_sphere(List_sphere)
{ };

float Scene::sdf_diff(Vector3 p) {
    float d = HUGE_VAL;
    float ma = HUGE_VAL;

    for (auto& s : list_sphere)
    {
        if (s.color.g == 31) {
            float buff = (p - s.center).norm() - s.radius;
            ma = std::min(ma, buff);
        }
        else
        {
            float buff = (p - s.center).norm() - s.radius;
            d = std::min(d, buff);
        }
    }

    return std::max(d, -ma);
}

float Scene::sdf_smooth(Vector3 p) {
    float d = HUGE_VAL;
    float d1 = HUGE_VAL;
    for (auto& s : list_sphere)
    {
        float new_d = (p - s.center).norm() - s.radius;
        if (d > new_d) {
            d1 = d;
            d = new_d; 
        }
        else if (new_d < d1) {
            d1 = new_d;
        }
    }

    float k = K_SDF_SMOOTH;
    if (std::abs(d - d1) < k) 
    {
        float h = std::max((k - std::abs(d-d1)) / k, 0.0f);
        return d - (h*h*h) * k / 6.0f;
    }
    else 
    {
        return d; 
    }

}

float Scene::sdf(Vector3 p) {
    float d = HUGE_VAL;
    for (auto& s : list_sphere)
    {
        d = std::min(d, (p - s.center).norm() - s.radius);
        // (p - s s.center).norm() -> direction
    }

    return d; 
}

Vector3 Scene::normal(Point3 P) {
    float eps = 0.01;
    float dx = sdf(P + Vector3(eps, 0, 0)) - sdf(P - Vector3(eps, 0, 0));
    float dy = sdf(P + Vector3(0, eps, 0)) - sdf(P - Vector3(0, eps, 0));
    float dz = sdf(P + Vector3(0, 0, eps)) - sdf(P - Vector3(0, 0, eps));
    Vector3 grad(dx, dy, dz);
    return grad / grad.norm();
}

Color Scene::find_color(Point3 inter_point)
{
    float total_weight = 0.0f;
    Color result(0, 0, 0);

    for (auto& sphere : list_sphere) {
        float dist = (inter_point - sphere.center).norm();
        float weight = 1.0f / (dist * dist);
        result = result + sphere.color * weight;
        total_weight += weight;
    }

    return result * (1.0f / total_weight);
}

Color Scene::shade(Point3 origin, Vector3 ray, float t)
{
    Point3 P = origin + ray * t; // intersection point
    Vector3 N = normal(P);
    //Vector3 R = (P - origin) / (P - origin).norm(); // ray
    //Vector3 S = R - N * (R.dot(N)) * 2; // rayon réfléchi
    Color res;
    for (auto& light : list_light)
    {
        Vector3 L = (light.origin - P) / (light.origin - P).norm(); // source lumineuse
        Color color_object = find_color(P);
        //Color color_object = Color(0,0,200);
        //Color color_object = Color(N.x, N.y, N.z);

        Color diffuse = color_object * std::max(0.0f, L.dot(N)) * 0.4;
        //Color specular = light.color * std::pow(std::max(0.0f, S.dot(L)), ns) * ks;
        // Color reflected = ray_march(S, P, n - 1) * kr;
        //res = res + specular + diffuse;
        res = res + diffuse;
    }

    return res + ambient_color;
}

float Scene::ray_march(Vector3 ray, Point3 origin)
{
    size_t ray_iteration = 20;
    float t = 0;
    for (size_t i = 0; i < ray_iteration; i += 1) {
        Point3 p = origin + ray * t;

        float d = sdf_smooth(p);
        if (d < k_close) {
            return t;
        }

        if (d > k_far) {
            return 0;
        }

        t += d;
    }

    return 0;
}


void Scene::ray_tracing(Image& image)
{
    // interpole tous les points du plan_image
    // trouve les vecteurs qui partent de l'origine
    Plan_image p_i = camera.plan_image;

    int height = p_i.p_image_height;
    int width = p_i.p_image_width;
    std::vector<Color> canvas(height * width);

    Point3 left = p_i.p_left_up;
    Point3 right = p_i.p_right_up;
    #pragma omp parallel for schedule(dynamic) // parallelism
    for (int y = 0; y < height; y++) {
        double t_y = y / (double)(height - 1);
        Point3 left_row = left + (p_i.p_left_down - left) * t_y;
        Point3 right_row = right + (p_i.p_right_down - right) * t_y;

        for (int x = 0; x < width; x++) {
            double t_x = x / (double)(width - 1);
            Point3 point = left_row + (right_row - left_row) * t_x;
            Vector3 ray = (point - camera.origin) / (point - camera.origin).norm();
            Color pixel = background_color;

            float dist = ray_march(ray, camera.origin);
            if (dist != 0) // touched an object
                pixel = shade(camera.origin, ray, dist);

            image.setPixel(x, y, pixel);
        }
    }
}

void Scene::update_density()
{
    for (auto& s : list_sphere)
    {
        s.update_density(s.center, list_sphere);
    }
}

// spike at r=0 
static float spiky_grad(float h, float r)
{
    if (r >= h || r < 1e-6f) return 0.0f;
    float diff = h - r;
    return (diff * diff) / (h * h * h);
}

void Scene::update_force()
{
    for (auto& s : list_sphere)
    {
        Vector3 force(0,0,0);
        for (auto& neigh : list_sphere)
        {
            if (&s == &neigh)
                continue;

            float dist = (s.center - neigh.center).norm();
            if (dist >= RADIUS_DENSITY || dist < 1e-6f)
                continue;

            Vector3 direction = (s.center - neigh.center) / dist; // from neigh -> s
            float grad = spiky_grad(RADIUS_DENSITY, dist);

            float pressure_term = (s.pressure + neigh.pressure) / (2.0f * neigh.density);
            force = force + (direction * MASS * pressure_term * grad);

            //std::cout << force << "\n";
        }
        s.force = force;
    }
}

void Scene::resolve_collision()
{
    for (size_t i = 0; i < list_sphere.size(); i++)
    {
        for (size_t j = i + 1; j < list_sphere.size(); j++)
        {
            auto& a = list_sphere[i];
            auto& b = list_sphere[j];

            float dist = (a.center - b.center).norm();
            float min_distance = (a.radius + b.radius) * MIN_DIST_SPHERE;

            if (dist < min_distance && dist > 0) {
                Vector3 direction = (a.center - b.center) / dist;
                float overlap = min_distance - dist;

                a.center = a.center + direction * overlap * 0.5f;
                b.center = b.center - direction * overlap * 0.5f;

                float vel_along_axis = (a.velocity - b.velocity).dot(direction);
                if (vel_along_axis < 0) {
                    //a.velocity = a.velocity - direction * vel_along_axis * 0.5f * (1 + restitution);
                    //b.velocity = b.velocity + direction * vel_along_axis * 0.5f * (1 + restitution);
                    a.velocity = a.velocity - direction * vel_along_axis * 0.5f;
                    b.velocity = b.velocity + direction * vel_along_axis * 0.5f;
                }
            }
        }
    }
}

float Scene::calculate_dt()
{
    float now = (float)glfwGetTime();
    if (last_time == 0.0f) last_time = now;
    float elapsed = now - last_time;
    last_time = now;
    if (elapsed > 0.05f) elapsed = 0.05f;
    return elapsed;
}

static constexpr float BIG_SPHERE_LIFETIME = 2.0f;
static constexpr float BIG_SPHERE_RADIUS   = 35.0f;

void Scene::add_drop(int i)
{
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<int> dist_x(-80, 80);
    static std::uniform_int_distribution<int> dist_z(-30, 30);
    int z = 540;
    if (i == 1) {
        z += dist_z(rng);
    }
    Sphere s(Point3(dist_x(rng), ceiling - SIZE_SPHERE - 1, 540),
             SIZE_SPHERE, Color(0, 0, 200));
    list_sphere.push_back(s);
}

void Scene::splash()
{
    Point3 impact(0, FLOOR, 540.0f);
    float radius = 100.0f;
    float strength = 1200.0f;

    for (auto& s : list_sphere)
    {
        float dist = (s.center - impact).norm();
        if (dist < radius && dist > 1e-4f)
        {
            Vector3 dir = (s.center - impact) / dist;
            dir.y = std::abs(dir.y) + 0.5f; // force composante vers le haut
            dir = dir / dir.norm();
            float falloff = 1.0f - (dist / radius);
            s.velocity = s.velocity + dir * strength * falloff;
        }
    }
}

void Scene::resize_spheres()
{
    for (auto& s : list_sphere)
        if (s.lifetime < 0)
            s.radius = SIZE_SPHERE;
}

void Scene::add_big_sphere()
{
    Sphere s(Point3(0, ceiling - BIG_SPHERE_RADIUS - 1, 540.0f), BIG_SPHERE_RADIUS, Color(255, 80, 0));
    s.velocity = Vector3(0, -500, 0);
    list_sphere.push_back(s);
}

void Scene::update()
{
    float elapsed = calculate_dt();
    time_accumulator += elapsed;

    int steps = 0;
    while (time_accumulator >= FIXED_DT && steps < MAX_SUBSTEPS)
    {
        update_density();
        update_force();
        for (auto& s : list_sphere)
        {
            s.update_pos(FIXED_DT);
        }
        resolve_collision();

        time_accumulator -= FIXED_DT;
        steps++;
    }
}

//void Scene::update()
//{
//    float dt = calculate_dt();
//
//    update_density();
//    update_force();
//    for (auto& s : list_sphere)
//    {
//        s.update_pos(dt);
//    }
//    resolve_collision();
//}
