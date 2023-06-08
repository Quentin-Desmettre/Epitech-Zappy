#include <raylib.h>
#include "venom.hpp"
#include "Mateyak/Window.hpp"
#include "Mateyak/Vector.hpp"
#include "Map.hpp"
#include "Mateyak/Sprite.hpp"
#include "Mateyak/Shaders.hpp"
#include "Client/client.hpp"
#include "Mateyak/Vector2.hpp"
#include "Mateyak/Model3D.hpp"
#include "Graphic.hpp"

void Graphic::loop(Mateyak::Vec2f mapSize)
{
    Mateyak::Window win(1920 / 2, 1080 / 2, "Zappy", 500);
    int seed = rand();
    Mateyak::Camera cam({5.0f, 5.0f, 5.0f}, {0.0f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f);
    Mateyak::Model3D sky(GenMeshSphere(200, 8, 8), Mateyak::Vec3f{0, 0, 0}, 1.0f, BLACK);
    Map map({400, 400}, 0.5);
    Mateyak::Sprite mapMdl = map.getMap();
    Mateyak::Sprite color = map.getColor();
    Venom ven;
    Mesh mesh = GenMeshHeightmap(mapMdl, (Vector3){100, 1, 100});
    Mateyak::Model3D model(mesh, Mateyak::Vec3f{-50, -0.5, -50});
    Mateyak::Model3D rock("assets/rock.obj", Mateyak::Vec3f{0, 0, 0}, 0.5f, RED);
    Mateyak::Model3D flat(GenMeshPoly(10, 10000.0f), Mateyak::Vec3f{-500, -1, -500}, 1.0f, BLACK);
    Mateyak::Shaders shader("src/gui/shader/base_lighting.vs", "src/gui/shader/test.fs");
    Font font = LoadFont("assets/arial.ttf");
    if (!font.texture.id) {
        std::cerr << "Error loading font" << std::endl;
        font = GetFontDefault();
    }

    model.setTexture(color);
    shader.setUniform("fogDensity", 0.02f);
    shader.setUniform("lightsPos", {0.0f, 20.0f, 0.0f});
    shader.setUniform("lightsColor", {0.5f, 0.5f, 0.5f});
    shader.setUniform("lightsEnabled", 1);
    int viewPos = shader.getUniformLocation("viewPos");
    model.setShader(shader);
    flat.setShader(shader);
    rock.setShader(shader);
    bool shaderEnabled = true;
    shader.setUniform("shaderEnabled", shaderEnabled);
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F1)) {
            shaderEnabled = !shaderEnabled;
            shader.setUniform("shaderEnabled", shaderEnabled);
        }
        shader.setUniform(viewPos, cam._position);
        ven.move_ven(cam.getRayCam());
        cam.Update();
        win.startDrawing();
        ClearBackground(Color{255 / 10, 255 / 20, 255 / 20, 255});
        win.begin3D(cam);
        BeginShaderMode(shader);
        Mateyak::Window::draw(model);
        Mateyak::Window::draw(sky);
        Mateyak::Window::draw(flat);
        Mateyak::Window::draw(rock);
        EndShaderMode();
        ven.draw_ven(seed, cam);
        _serverInformations.startComputing();
        for (auto &it : _serverInformations.getPlayers()) {
            _serverInformations.updatePlayer(it);
            it->ven.draw_ven(seed, cam);
        }
        _serverInformations.endComputing();

        DrawGrid(10, 10);
        win.end3D();
        DrawFPS(10, 10);
        win.endDrawing();
    }
    UnloadFont(font);
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
    Graphic graphic;

    try {
        ErrorHandling errorHandling(ac, av);
        errorHandling.parse();
        GuiClient client(graphic.getServerInformations(), errorHandling.getIp(), errorHandling.getPort());

        if (!client.CheckValidServer())
            return 84;
        Mateyak::Vec2f mapSize = graphic.getServerInformations().getMapSize();

       std::thread t(&GuiClient::compute, &client);
        graphic.loop(mapSize);
        client.stop();
        t.join();

    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
