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


find_package(raylib QUIET)
include_directories(${RAYLIB_INCLUDE_DIRS})

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
        src/server/utility/str_array_2.c
        src/server/trantor/map.c
        src/server/network/ai_log_handler.c
        src/server/utility/linked_list/multi_dimensional_linked_list/liked_list.c
        src/server/utility/linked_list/multi_dimensional_linked_list/remove_if.c
        src/server/trantor/tile_handling.c src/server/trantor/tile_handling2.c)

set_target_properties(zappy_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
