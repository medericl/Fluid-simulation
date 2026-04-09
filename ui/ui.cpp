#include "ui.hh"
#include "../src/config.hh"
#include "imgui.h"

static void color_edit(const char* label, Color& c)
{
    float col[3] = { c.r / 255.f, c.g / 255.f, c.b / 255.f };
    if (ImGui::ColorEdit3(label, col)) {
        c.r = col[0] * 255.f;
        c.g = col[1] * 255.f;
        c.b = col[2] * 255.f;
    }
}

void render_ui(Scene& scene)
{
    ImGui::Begin("Simulation");

    if (ImGui::CollapsingHeader("Simulation")) {
        ImGui::Text("Particles: %zu", scene.list_sphere.size());
        ImGui::Text("WIDTH: %d  HEIGHT: %d", WIDTH, HEIGHT);
        if (ImGui::SliderInt("Size sphere", &SIZE_SPHERE, 1, 30))
            scene.resize_spheres();
        ImGui::SliderInt("Camera zoom",  &CAMERA_ZOOM,   10, 500);
    }

    if (ImGui::CollapsingHeader("Ray")) {
        ImGui::SliderFloat("kd",      &kd,      0.0f, 1.0f);
        ImGui::SliderFloat("ks",      &ks,      0.0f, 1.0f);
        ImGui::SliderFloat("kr",      &kr,      0.0f, 1.0f);
        ImGui::SliderFloat("ns",      &ns,      1.0f, 100.0f);
        ImGui::SliderInt("rec ray",   &rec_ray, 1, 20);
        ImGui::SliderFloat("k_close", &k_close, 0.0001f, 1.0f);
        ImGui::SliderFloat("k_far",   &k_far,   100.0f, 2000.0f);
        color_edit("Ambient",    ambient_color);
        color_edit("Background", background_color);
        color_edit("Light",      color_light);
    }

    if (ImGui::CollapsingHeader("Bordure")) {
        ImGui::SliderFloat("Floor",   &FLOOR,         -500.0f, 0.0f);
        ImGui::SliderFloat("Ceiling", &ceiling,        0.0f, 500.0f);
        ImGui::SliderFloat("Left",    &left_bordure,  -500.0f, 0.0f);
        ImGui::SliderFloat("Right",   &right_bordure,  0.0f, 500.0f);
        ImGui::SliderFloat("Front",   &FRONT_BORDURE,  300.0f, 540.0f);
        ImGui::SliderFloat("Back",    &BACK_BORDURE,   540.0f, 800.0f);
    }

    if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderFloat("Gravity",        &GRAVITY,        0.0f, 1000.0f);
        ImGui::SliderFloat("Restitution",    &restitution,    0.0f, 1.0f);
        ImGui::SliderFloat("Min dist",       &MIN_DIST_SPHERE,0.1f, 5.0f);
        ImGui::SliderFloat("Mass",           &MASS,           0.1f, 10.0f);
        ImGui::SliderFloat("K Pressure",     &K_PRESSURE,     0.0f, 10000.0f);
        //ImGui::SliderFloat("Density ref",    &DENSITY_ID,     0.0f, 5.0f);
        ImGui::SliderFloat("Density ref",    &DENSITY_ID,     0.0f, 5.0f);
        ImGui::SliderFloat("Radius density", &RADIUS_DENSITY, 1.0f, 300.0f);
        ImGui::SliderFloat("K SDF Smooth",   &K_SDF_SMOOTH,   0.0f, 100.0f);
        // ImGui::SliderFloat("Fixed DT",      &FIXED_DT,       0.0001f, 0.01f, "%.4f");
        // ImGui::SliderInt("Max substeps",    &MAX_SUBSTEPS,    1, 50);
    }

    ImGui::Separator();
    ImGui::Text("[B] Big sphere  [S] Splash  [SPACE] Drop");

    ImGui::End();
}
