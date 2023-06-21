/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** main
*/

#include <stddef.h>
#include <string.h>
#include "server.h"
#include "utility/strings.h"
#include <signal.h>

server_t **get_server(void)
{
    static server_t *server = NULL;

    return &server;
}

void handle_sig(int sig)
{
    (void)sig;
    debug("Signal received, shutting down server\n", 38);
    destroy_server(*get_server());
    exit(0);
}

void handle_all_sigs(void)
{
    for (int i = 1; i < 32; i++) {
        if (i != SIGSEGV && i != SIGFPE && i != SIGKILL && i != SIGSTOP)
            signal(i, handle_sig);
    }
}

int help(void)
{
    safe_write(1,
    "\nUSAGE: ./zappy_server -p port -x width -y height -n name1 name2 ..."
    " -c clientsNb -f freq\n"
        "\tport\t\tis the port number\n"
        "\twidth\t\tis the width of the world\n"
        "\theight\t\tis the height of the world\n"
        "\tnameX\t\tis the name of the team X\n"
        "\tclientsNb\tis the number of authorized clients per team\n"
        "\tfreq\t\tis the reciprocal of time unit for execution of actions\n\n",
    340);
    return 0;
}

int main(int ac, char **av)
{
    char *err = NULL;
    server_t *server = init_server(ac, av, &err);

    if (server && server->params.help)
        return help();
    if (!server) {
        if (err) {
            safe_write(2, err, strlen(err));
            safe_write(2, "\n", 1);
        }
        help();
        return 84;
    }
    *get_server() = server;
    srandom(time(NULL));
    handle_all_sigs();
    run_server(server);
    destroy_server(server);
    return 0;
}
