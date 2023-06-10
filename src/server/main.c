/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** main
*/

#include <stddef.h>
#include <string.h>
#include "server.h"
#include <signal.h>

void handle_sig(int sig)
{
    (void)sig;
    safe_write(1, "Signal received, shutting down server\n", 38);
    exit(0);
}

void handle_all_sigs(void)
{
    for (int i = 1; i < 32; i++) {
        if (i != SIGSEGV && i != SIGFPE && i != SIGKILL && i != SIGSTOP)
            signal(i, handle_sig);
    }
}

int main(int ac, char **av)
{
    char *err = NULL;
    server_t *server = init_server(ac, av, &err);

    if (!server) {
        safe_write(2, err, strlen(err));
        safe_write(2, "\n", 1);
        return 84;
    }
    srandom(time(NULL));
    handle_all_sigs();
    run_server(server);
    destroy_server(server);
    return 0;
}
