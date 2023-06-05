// #include "Include.hpp"
#include <raylib.h>
#include "venom.hpp"
#include "Mateyak/Window.hpp"
#include "Mateyak/Vector.hpp"
#include "Map.hpp"
//#include "Mateyak/rlights.h"
#include "Mateyak/Sprite.hpp"
#include "Mateyak/Shaders.hpp"

int main(void)
{
    Mateyak::Window win;
    int seed = rand();
    Mateyak::Camera cam({5.0f, 5.0f, 5.0f}, {0.0f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f);
    Model sky = LoadModelFromMesh(GenMeshSphere(200, 8, 8));

    Map map({400, 400}, 0.5);
    Mateyak::Sprite mapMdl = map.getMap();
    Mateyak::Sprite color = map.getColor();
    Venom ven;
    Mesh mesh = GenMeshHeightmap(mapMdl, (Vector3){100, 1, 100});
    Model model = LoadModelFromMesh(mesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = color;
    Model flat = LoadModelFromMesh(GenMeshPoly(10, 10000.0f));
    Mateyak::Shaders shader("shader/base_lighting.vs", "shader/fog.fs");
    shader.setUniform("fogDensity", 0.02f);
    int viewPos = shader.getUniformLocation("viewPos");
    flat.materials[0].shader = shader;
    model.materials[0].shader = shader;
    while (!WindowShouldClose()) {
        shader.setUniform(viewPos, cam._position);
        ven.move_ven(cam.getRayCam());
        cam.Update();
        win.startDrawing();
            ClearBackground(Color{255 / 10, 255 / 20, 255 / 20, 255});
            win.begin3D(cam);
                BeginShaderMode(shader);
                DrawModel(model, Vector3{-50, -0.5, -50}, 1.0f, WHITE);
                DrawModel(sky, Vector3{0, 0, 0}, 1.0f, WHITE);
                DrawModel(flat, Vector3{-500, -1, -500}, 1.0f, BLACK);
                EndShaderMode();
                ven.draw_ven(seed);
                DrawGrid(30, 10/3.f);
            win.end3D();
            DrawFPS(10, 10);
        win.endDrawing();
    }
    return 0;
}
