cmake_minimum_required(VERSION 3.22)
project(Epitech-Zappy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS "-Wall -Wextra")
set(PAULIIINNN zappy_gui)

if (APPLE) # Mac OS X
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers -Wno-unused-command-line-argument")
endif ()

include_directories(include)
include_directories(include/gui)
include_directories(include/server)

find_package(Boost QUIET)
include_directories(${Boost_INCLUDE_DIR})

find_package(raylib QUIET)
include_directories(${RAYLIB_INCLUDE_DIRS})

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Please install boost!\nOn Ubuntu: sudo apt install libboost-all-dev\nOn Mac OS X: brew install boost\nOn Windows: https://www.boost.org/doc/libs/1_75_0/more/getting_started/windows.html")
endif ()

if (NOT raylib_FOUND)
    message(FATAL_ERROR "Please install raylib!\nOn Ubuntu: sudo apt install libraylib-dev\nOn Mac OS X: brew install raylib\nOn Windows:")
endif ()

add_executable(zappy_gui
        src/gui/Client/client.cpp
        src/gui/Client/client_loop.cpp
        src/gui/Client/receive.cpp
        src/gui/Mateyak/Camera.cpp
        src/gui/Mateyak/Matrix.cpp
        src/gui/Mateyak/Shaders.cpp
        src/gui/Mateyak/Sprite.cpp
        src/gui/Mateyak/Triangle.cpp
        src/gui/Mateyak/Window.cpp
        src/gui/Mateyak/Model3D.cpp
        src/gui/Perlin/Perlin.cpp
        src/gui/main.cpp
        src/gui/Map.cpp
        src/gui/Utils3d.cpp
        src/gui/venom.cpp
        src/gui/Informations/Player.cpp src/gui/Informations/ServerInformations.cpp src/gui/Client/setter.cpp include/gui/Informations/Player.hpp include/gui/Informations/ServerInformations.hpp)

target_link_libraries(zappy_gui ${raylib_LIBRARY} ${Boost_LIBRARY})
set_target_properties(zappy_gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_command(TARGET zappy_gui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${PAULIIINNN} ${CMAKE_SOURCE_DIR}/zappy_ai
)

add_executable(zappy_server
        src/server/parse_args.c
        src/server/strings.c
        src/server/network/server_run.c
        src/server/network/server_init.c
        src/server/linked_list/linked_list.c
        src/server/main.c
        src/server/trantor/trantor_init.c
)

set_target_properties(zappy_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
