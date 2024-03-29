cmake_minimum_required(VERSION 3.22)
project(Epitech-Zappy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS "-Wall -Wextra")
if (APPLE)
    set(PAULIIINNN src/ai/main_mac.py)
else ()
    set(PAULIIINNN src/ai/main.py)
endif ()

if (APPLE) # Mac OS X
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers -Wno-unused-command-line-argument")
endif ()

add_custom_target( unZip ALL)
add_custom_command(TARGET unZip PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/fmod
        COMMAND ${CMAKE_COMMAND} -E tar xzvf ${CMAKE_SOURCE_DIR}/fmod.tar.gz
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS ${CMAKE_SOURCE_DIR}/fmod.tar.gz
        COMMENT "Unpacking abc.zip"
        VERBATIM)

include_directories(include)
include_directories(include/gui)
include_directories(include/server)

if (APPLE)
    include_directories(fmod/inc)
    set(FMOD_INCLUDE_DIR "fmod/inc")
    set(FMOD_LIBRARY "fmod/lib")
else ()
    include_directories(fmod/include)
    set(FMOD_INCLUDE_DIR "fmod/include")
    set(FMOD_LIBRARY "fmod/libs")
endif ()

set(FMOD_LIBRARIES "-lfmod")

include_directories(${FMOD_INCLUDE_DIR})
link_directories(${FMOD_LIBRARY})

find_package(raylib QUIET)
include_directories(${RAYLIB_INCLUDE_DIRS})
find_package(Boost COMPONENTS system filesystem QUIET)
include_directories(${Boost_INCLUDE_DIR})

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Please install boost!\nsudo apt install build-essential git cmake -y\nsudo apt install libboost-all-dev -y")
endif ()

if (NOT raylib_FOUND)
    message(FATAL_ERROR "Please install raylib!\nsudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev -y\ngit clone https://github.com/raysan5/raylib.git raylib\ncd raylib\nmkdir build && cd build\ncmake -DBUILD_SHARED_LIBS=ON ..\nmake\nsudo make install\ncd ../../\nrm -rf raylib")
endif ()

add_executable(zappy_gui
        src/gui/Map.cpp
        src/gui/Mateyak/Camera.cpp
        src/gui/Mateyak/Model3D.cpp
        src/gui/Mateyak/Window.cpp
        src/gui/Mateyak/Sprite.cpp
        src/gui/Mateyak/Triangle.cpp
        src/gui/Mateyak/Matrix.cpp
        src/gui/Mateyak/Shaders.cpp
        src/gui/venom.cpp
        src/gui/Informations/Player.cpp
        src/gui/Informations/Team.cpp
        src/gui/Informations/ServerInformations.cpp
        src/gui/Perlin/Perlin.cpp
        src/gui/Client/receive.cpp
        src/gui/Client/client.cpp
        src/gui/Client/setter.cpp
        src/gui/Client/client_loop.cpp
        src/gui/Utils3d.cpp
        src/gui/main.cpp
        src/gui/venom.cpp
        src/gui/Informations/Player.cpp src/gui/Informations/ServerInformations.cpp src/gui/Client/setter.cpp include/gui/Informations/Player.hpp include/gui/Informations/ServerInformations.hpp src/gui/Mateyak/Model3D.cpp include/gui/Graphic.hpp src/gui/Informations/Team.cpp include/gui/Informations/Team.hpp src/gui/Graphic/Display.cpp src/gui/Graphic/Compute.cpp src/gui/ErrorHandling/ErrorHandling.cpp include/gui/ErrorHandling.hpp
        include/gui/Mateyak/Audio.hpp src/gui/Mateyak/Audio.cpp src/gui/Menu/menu.cpp)

target_link_libraries(zappy_gui ${raylib_LIBRARY} ${Boost_LIBRARY} ${FMOD_LIBRARIES})
set_target_properties(zappy_gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})


add_custom_command(TARGET zappy_gui PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${PAULIIINNN} ${CMAKE_SOURCE_DIR}/zappy_ai
)

add_executable(zappy_server
        src/server/network/server_run_utils.c
        src/server/network/notify_gui.c
        src/server/network/check_food.c
        src/server/network/gui_connected_answer.c
        src/server/network/ai_log_handler.c
        src/server/network/client.c
        src/server/network/handle_actions.c
        src/server/network/client_input.c
        src/server/network/state_handler.c
        src/server/network/server_run.c
        src/server/network/server_init.c
        src/server/main.c
        src/server/trantor/action.c
        src/server/trantor/tile_handling.c
        src/server/trantor/tile_handling2.c
        src/server/trantor/map.c
        src/server/trantor/player.c
        src/server/trantor/data_send.c
        src/server/trantor/trantor_init.c
        src/server/trantor/ai_handlers/ai_handlers2.c
        src/server/trantor/ai_handlers/incantation.c
        src/server/trantor/ai_handlers/incantation_utils.c
        src/server/trantor/ai_handlers/ai_handlers1.c
        src/server/trantor/ai_handlers/fork.c
        src/server/trantor/ai_handlers/ai_handlers3.c
        src/server/trantor/ai_handlers/freeze.c
        src/server/trantor/tile_handling3.c
        src/server/trantor/gui_handling/handlers2.c
        src/server/trantor/gui_handling/handlers1.c
        src/server/trantor/gui_handling/handlers3.c
        src/server/trantor/team.c
        src/server/parse_args.c
        src/server/check_args_validity.c
        src/server/utility/strings2.c
        src/server/utility/debug_print.c
        src/server/utility/str_array.c
        src/server/utility/time.c
        src/server/utility/str_array_2.c
        src/server/utility/linked_list/remove_if.c
        src/server/utility/linked_list/multi_dimensional_linked_list/dim_liked_list.c
        src/server/utility/linked_list/linked_list.c
        src/server/utility/safe_write/packet.c
        src/server/utility/safe_write/my_write.c
        src/server/utility/garbage_collector/garbage_handling.c
        src/server/utility/garbage_collector/my_malloc.c
        src/server/utility/strings.c
)

find_library(MATH_LIBRARY m)

if(MATH_LIBRARY)
    target_link_libraries(zappy_server PUBLIC ${MATH_LIBRARY})
endif()
set_target_properties(zappy_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_dependencies(zappy_server unZip)
add_dependencies(zappy_gui unZip)

if (NOT APPLE)
    add_executable(server_tests
            src/server/network/server_run_utils.c
            src/server/network/notify_gui.c
            src/server/network/check_food.c
            src/server/network/gui_connected_answer.c
            src/server/network/ai_log_handler.c
            src/server/network/client.c
            src/server/network/handle_actions.c
            src/server/network/client_input.c
            src/server/network/state_handler.c
            src/server/network/server_run.c
            src/server/network/server_init.c
            src/server/main.c
            src/server/trantor/action.c
            src/server/trantor/tile_handling.c
            src/server/trantor/tile_handling2.c
            src/server/trantor/map.c
            src/server/trantor/player.c
            src/server/trantor/data_send.c
            src/server/trantor/trantor_init.c
            src/server/trantor/ai_handlers/ai_handlers2.c
            src/server/trantor/ai_handlers/incantation.c
            src/server/trantor/ai_handlers/incantation_utils.c
            src/server/trantor/ai_handlers/ai_handlers1.c
            src/server/trantor/ai_handlers/fork.c
            src/server/trantor/ai_handlers/ai_handlers3.c
            src/server/trantor/ai_handlers/freeze.c
            src/server/trantor/tile_handling3.c
            src/server/trantor/gui_handling/handlers2.c
            src/server/trantor/gui_handling/handlers1.c
            src/server/trantor/gui_handling/handlers3.c
            src/server/trantor/team.c
            src/server/parse_args.c
            src/server/check_args_validity.c
            src/server/utility/strings2.c
            src/server/utility/debug_print.c
            src/server/utility/str_array.c
            src/server/utility/time.c
            src/server/utility/str_array_2.c
            src/server/utility/linked_list/remove_if.c
            src/server/utility/linked_list/multi_dimensional_linked_list/dim_liked_list.c
            src/server/utility/linked_list/linked_list.c
            src/server/utility/safe_write/packet.c
            src/server/utility/safe_write/my_write.c
            src/server/utility/garbage_collector/garbage_handling.c
            src/server/utility/garbage_collector/my_malloc.c
            src/server/utility/strings.c
            tests/server/test_tile_handling.c
            tests/server/test_tile_handling2.c
            tests/server/test_incantation_utils.c
            tests/server/test_server_init.c
            tests/server/test_tile_handling3.c
            tests/server/test_ai_log.c
            tests/server/test_strings.c
            tests/server/test_ai_handlers.c
            tests/server/test_actions.c
            tests/server/test_handle_actions.c
            tests/server/test_linked_list.c
            tests/server/test_fork.c
            tests/server/test_parse_args.c
            tests/server/test_syscalls.c
            tests/server/test_safe_write.c
            tests/server/test_utility.c
            tests/server/test_time.c)
    if(MATH_LIBRARY)
        target_link_libraries(server_tests PUBLIC criterion ${MATH_LIBRARY})
    endif()
endif ()
