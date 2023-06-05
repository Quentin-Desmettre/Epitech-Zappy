// #include "Include.hpp"
#include <raylib.h>
#include "venom.hpp"
#include "Mateyak/Window.hpp"
#include "Mateyak/Vector.hpp"
#include "Map.hpp"
#include "Mateyak/Sprite.hpp"
#include "Mateyak/Shaders.hpp"
#include "Client/client.hpp"
#include "Mateyak/Vector2.hpp"


class Player
{
    enum STATE {INCANTING, DEAD, EGG, BROADCASTING, DROPING, TAKING};

    public:
        Player() = default;
        ~Player() = default;
    private:
        Mateyak::Vec2f position;
        Mateyak::Vec2f nextPosition;
        int level;
        int inventory[7];
        int orientation;
        int team;
        STATE state;
        int eggTime;
        std::string broadcastMessage;
};


class ServerInformations
{
    public:
        ServerInformations() = default;
        ~ServerInformations() = default;
    private:
        Mateyak::Vec2f mapSize;
        std::vector<std::vector<int>> map;
        std::vector<std::string> teams;
        std::vector<Player> players;
};

int main(int ac, char **av)
{
    std::vector<std::string> args(av, av + ac);

    if (args.size() > 1 && args.size() < 5) {
        std::cerr << "USAGE: ./zappy_ai -p port -h machine" << std::endl;
        std::cerr << "\tport is the port number" << std::endl;
        std::cerr << "\tmachine is the name of the machine; localhost by default" << std::endl;
        return 84;
    }

    //std::cout << "Awaiting connection" << std::endl;
    //Client client("127.0.0.1", 4242);
//
    //if (client.connect_client()) {
    //    std::cout << "Not connected to a nice server" << std::endl;
    //    return 84;
    //}
    //client.receive_message();
//
    //std::cout << "Game can start now" << std::endl;
    //std::thread t1(computeClient, std::ref(client));

    Mateyak::Window win(1920 / 2, 1080 / 2, "Zappy", 500);
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
    //t1.join();
    return 0;
}

//(bct) ([0-9]+ ){8}[0-9]+
//(msz) [0-9]+ [0-9]+
//(sgt) [0-9]+
//(tna) [a-zA-Z0-9]+
