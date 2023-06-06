// #include "Include.hpp"
#include <raylib.h>
#include "venom.hpp"
#include "Mateyak/Window.hpp"
#include "Mateyak/Vector.hpp"
#include "Map.hpp"
#include "Mateyak/Sprite.hpp"
#include "Mateyak/Shaders.hpp"
#include "Client/client.hpp"


void graph()
{
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
        //DrawGrid(30, 10/3.f);
        win.end3D();
        DrawFPS(10, 10);
        win.endDrawing();
    }
}

class ErrorHandling {
    public:
        class Error : public std::exception {
            public:
                Error(std::string const &message) : _message(message) {}
                const char *what() const noexcept override { return _message.c_str(); }
            private:
                std::string _message;
        };
        void parse();
        ErrorHandling(int ac, char **av);
        std::string getPort() const;
        std::string getIp() const;
        ~ErrorHandling() = default;
    private:
        int _ac;
        char **_av;
        std::string _port;
        std::string _ip;
};

ErrorHandling::ErrorHandling(int ac, char **av) : _ac(ac), _av(av)
{
    if (!(ac == 3 || ac == 5))
        throw Error("Invalid number of arguments");
}

void ErrorHandling::parse()
{
    for (int i = 1; i < _ac; i++) {
        if (std::string(_av[i]) == "-p") {
            if (i + 1 >= _ac)
                throw Error("Port not defined after -p");
            _port = _av[i + 1];
            i++;
        }
        else if (std::string(_av[i]) == "-h") {
            if (i + 1 >= _ac)
                throw Error("Ip not defined after -h");
            _ip = _av[i + 1];
            i++;
        }
        else {
            std::string test(_av[i]);
            throw Error(test + std::string(" is not a valid argument"));
        }
    }
    if (_port.empty())
        throw Error("Port not defined");
}

std::string ErrorHandling::getPort() const
{
    return _port;
}

std::string ErrorHandling::getIp() const
{
    if (_ip.empty())
        return std::string("localhost");
    return _ip;
}

int main(int ac, char **av)
{
    ServerInformations serverInformations;

    try {
        ErrorHandling errorHandling(ac, av);
        errorHandling.parse();
        GuiClient client(serverInformations, errorHandling.getIp(), errorHandling.getPort());

        if (!client.CheckValidServer())
            return 84;

        std::thread t(&GuiClient::compute, &client);
        graph();
        client.stop();
        t.join();

    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
