#include "venom.hpp"
#include "Map.hpp"
#include "ErrorHandling.hpp"

int main(int ac, char **av)
{
    try {
        ErrorHandling errorHandling(ac, av);
        errorHandling.parse();
        errorHandling.Run();
    } catch (const std::exception& ex) {
        std::string error = ex.what();
        if (error == "Connection failed") {
            std::cerr << "Please verify if the server is started" << std::endl;
        } else {
            std::cerr << "Exception: " << ex.what() << std::endl;
            std::cout << "USAGE:\t./zappy_ai -p port -h machine" << std::endl;
            std::cout << "\tport\tis the port number" << std::endl;
            std::cout << "\tmachine\tis the name of the machine; localhost by default" << std::endl;
        }
    }
    return 0;
}
