cmake_minimum_required(VERSION 3.25)
project(Epitech_Zappy)

set(CMAKE_C_STANDARD 23)

include_directories(include)
include_directories(include/server)

add_executable(Epitech_Zappy
        src/gui/main.cpp
        src/server/main.c include/server/server.h src/server/utility/linked_list/linked_list.c src/server/utility/strings.c src/server/parse_args.c include/server/utility/strings.h include/server/args.h src/server/network/server_init.c src/server/network/server_run.c src/server/trantor/trantor_init.c include/server/trantor.h src/server/utility/safe_write/my_write.c src/server/utility/safe_write/packet.c include/server/utility/safe_write.h src/server/utility/linked_list/remove_if.c include/server/utility/garbage_collector.h src/server/utility/garbage_collector/my_malloc.c src/server/utility/garbage_collector/garbage_handling.c src/server/utility/time.c src/server/utility/str_array.c src/server/trantor/action.c src/server/network/client_input.c src/server/network/state_handler.c src/server/trantor/gui_handling/handlers1.c src/server/trantor/gui_handling/handlers2.c src/server/trantor/gui_handling/responses1.c src/server/trantor/player.c src/server/trantor/ai_handlers/ai_handlers1.c src/server/trantor/ai_handlers/ai_handlers2.c src/server/trantor/ai_handlers/ai_handlers3.c tests/server/test_utility.c tests/server/test_linked_list.c tests/server/test_strings.c src/server/utility/str_to_word_array.c tests/server/test_syscalls.c tests/server/test_safe_write.c tests/server/test_parse_args.c)
