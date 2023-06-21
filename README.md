# ZAPPY - EPITECH 2022-2023

### Authors
- [**Quentin Desmettre**](https://github.com/Quentin-Desmettre)
- [**Paulin Leclercq**](https://github.com/Paulin-Leclercq)
- [**Edgar Lecuyer**](https://github.com/EdgarLec)
- [**Ewen Talleux**](https://github.com/TalleuxEwen)
- [**Noa Trachez**](https://github.com/Noa-Trachez)
- [**Clément Vandeville**](https://github.com/ClementVand)

### Description
Zappy is a project that aims to create a game in which you have to survive on a planet with your team. You will have to collect resources, food in order to evolve or to be the last team alive.


### Libraries
- [**Cmake**](https://cmake.org/)
- [**Doxygen**](https://www.doxygen.nl/index.html)

#### GUI
- [**G++**](https://gcc.gnu.org/)
- [**Boost**](https://www.boost.org/)
- [**Raylib**](https://www.raylib.com/)
- [**FMOD (This dependency will be installed automatically)**](https://www.fmod.com/)

#### SERVER
- [**GCC**](https://gcc.gnu.org/)

#### AI
- [**Python3**](https://www.python.org/)
- [**Regex**](https://docs.python.org/3/library/re.html)
- [**Unittest**](https://docs.python.org/3/library/unittest.html)
- [**Coverage**](https://coverage.readthedocs.io/en/coverage-5.5/)

#### DOCUMENTATION
- [**Doxygen**](https://www.doxygen.nl/index.html)
- [**Latex**](https://www.latex-project.org/)

### How to use
- Clone the repository
- Install the dependencies: boost, raylib, fmod (Those steps are described in the CMakelists.txt)
- Run `run.sh` to compile the project
- Run `./zappy_server -p [port] -x [width] -y [height] -n [team1] [team2] ... -c [clientsNb] -f [freq]` to launch the server
- Run `./zappy_ai -p [port] -n [team]` to launch the AI
- Run `./zappy_graphic -p [port]` or  `./zappy_graphic` to launch the graphic client

