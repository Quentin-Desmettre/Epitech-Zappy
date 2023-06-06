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
include_directories(/usr/local/include)

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
        src/gui/Client/send.cpp
        src/gui/Mateyak/Camera.cpp
        src/gui/Mateyak/Matrix.cpp
        src/gui/Mateyak/Shaders.cpp
        src/gui/Mateyak/Sprite.cpp
        src/gui/Mateyak/Triangle.cpp
        src/gui/Mateyak/Window.cpp
        src/gui/Perlin/Perlin.cpp
        src/gui/main.cpp
        src/gui/Map.cpp
        src/gui/Utils3d.cpp
        src/gui/venom.cpp
)

target_link_libraries(zappy_gui ${raylib_LIBRARY} ${Boost_LIBRARY})
set_target_properties(zappy_gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_command(TARGET zappy_gui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${PAULIIINNN} ${CMAKE_SOURCE_DIR}/zappy_ai
)

add_executable(zappy_server
        src/server/network/notify_gui.c
        src/server/network/handle_actions.c
        src/server/network/client_input.c
        src/server/network/state_handler.c
        src/server/network/server_run.c
        src/server/network/server_init.c
        src/server/main.c
        src/server/trantor/action.c
        src/server/trantor/player.c
        src/server/trantor/trantor_init.c
        src/server/trantor/ai_handlers/ai_handlers2.c
        src/server/trantor/ai_handlers/ai_handlers1.c
        src/server/trantor/ai_handlers/ai_handlers3.c
        src/server/trantor/gui_handling/handlers2.c
        src/server/trantor/gui_handling/handlers1.c
        src/server/trantor/gui_handling/responses1.c
        src/server/parse_args.c
        src/server/utility/str_array.c
        src/server/utility/time.c
        src/server/utility/str_to_word_array.c
        src/server/utility/linked_list/remove_if.c
        src/server/utility/linked_list/linked_list.c
        src/server/utility/safe_write/packet.c
        src/server/utility/safe_write/my_write.c
        src/server/utility/garbage_collector/garbage_handling.c
        src/server/utility/garbage_collector/my_malloc.c
        src/server/utility/strings.c
        src/server/utility/str_array_2.c)

set_target_properties(zappy_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(server_tests
        src/server/network/notify_gui.c
        src/server/network/handle_actions.c
        src/server/network/client_input.c
        src/server/network/state_handler.c
        src/server/network/server_run.c
        src/server/network/server_init.c
        src/server/trantor/action.c
        src/server/trantor/player.c
        src/server/trantor/trantor_init.c
        src/server/trantor/ai_handlers/ai_handlers2.c
        src/server/trantor/ai_handlers/ai_handlers1.c
        src/server/trantor/ai_handlers/ai_handlers3.c
        src/server/trantor/gui_handling/handlers2.c
        src/server/trantor/gui_handling/handlers1.c
        src/server/trantor/gui_handling/responses1.c
        src/server/parse_args.c
        src/server/utility/str_array.c
        src/server/utility/time.c
        src/server/utility/str_to_word_array.c
        src/server/utility/linked_list/remove_if.c
        src/server/utility/linked_list/linked_list.c
        src/server/utility/safe_write/packet.c
        src/server/utility/safe_write/my_write.c
        src/server/utility/garbage_collector/garbage_handling.c
        src/server/utility/garbage_collector/my_malloc.c
        src/server/utility/strings.c
        src/server/utility/str_array_2.c
        tests/server/test_strings.c
        tests/server/test_ai_handlers.c
        tests/server/test_linked_list.c
        tests/server/test_parse_args.c
        tests/server/test_syscalls.c
        tests/server/test_safe_write.c
        tests/server/test_utility.c
        tests/server/test_actions.c tests/server/test_handle_actions.c tests/server/test_server_init.c src/server/network/ai_log_handler.c)
target_link_libraries(server_tests --coverage -lcriterion -g)
set_target_properties(zappy_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
