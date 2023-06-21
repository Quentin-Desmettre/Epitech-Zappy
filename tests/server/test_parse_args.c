/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** test_parse_args
*/

#include "args.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "utility/strings.h"
#include <unistd.h>

static void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

typedef struct {
    int port;
    int width;
    int height;
    char *names[2048];
    int slots;
    int freq;
} args_t_raw;

typedef struct {
    char *args[1024];
    args_t_raw expected;
    bool valid;
} arg_test_t;

const arg_test_t tests[] = {
        // Default
        {
            .args = {"./zappy_server", NULL},
            .expected = {
                    .port = DEFAULT_PORT,
                    .width = DEFAULT_WIDTH,
                    .height = DEFAULT_HEIGHT,
                    .names = {"Team1", "Team2", "Team3", "Team4", NULL},
                    .slots = DEFAULT_SLOTS,
                    .freq = DEFAULT_FREQ
            },
            .valid = true,
        },

        // Only port
        {
            .args = {"./zappy_server", "-p", "9999", NULL},
            .expected = {
                    .port = 9999,
                    .width = DEFAULT_WIDTH,
                    .height = DEFAULT_HEIGHT,
                    .names = {"Team1", "Team2", "Team3", "Team4", NULL},
                    .slots = DEFAULT_SLOTS,
                    .freq = DEFAULT_FREQ
            },
            .valid = true,
        },

        // Multiple definition of port
        {
            .args = {"./zappy_server", "-p", "9999", "-p", "13", NULL},
            .expected = {
                    .port = 13,
                    .width = DEFAULT_WIDTH,
                    .height = DEFAULT_HEIGHT,
                    .names = {"Team1", "Team2", "Team3", "Team4", NULL},
                    .slots = DEFAULT_SLOTS,
                    .freq = DEFAULT_FREQ
            },
            .valid = true,
        },

        // Width and height
        {
            .args = {"./zappy_server", "-x", "10", "-y", "20", NULL},
            .expected = {
                    .port = DEFAULT_PORT,
                    .width = 10,
                    .height = 20,
                    .names = {"Team1", "Team2", "Team3", "Team4", NULL},
                    .slots = DEFAULT_SLOTS,
                    .freq = DEFAULT_FREQ
            },
            .valid = true,
        },

        // -p without arg
        {
            .args = {"./zappy_server", "-p", NULL},
            .valid = false,
        },
        {
            .args = {"./zappy_server", "-p", "-x", "10", NULL},
            .valid = false,
        },

        // Too small width
        {
            .args = {"./zappy_server", "-x", "9", NULL},
            .valid = false,
        },

        // Too small height
        {
            .args = {"./zappy_server", "-y", "9", NULL},
            .valid = false,
        },

        // Too big width
        {
            .args = {"./zappy_server", "-x", "31", NULL},
            .valid = false,
        },

        // Too big height
        {
            .args = {"./zappy_server", "-y", "31", NULL},
            .valid = false,
        },

        // Only width
        {
            .args = {"./zappy_server", "-x", "27", NULL},
            .expected = {
                    .port = DEFAULT_PORT,
                    .width = 27,
                    .height = DEFAULT_HEIGHT,
                    .names = {"Team1", "Team2", "Team3", "Team4", NULL},
                    .slots = DEFAULT_SLOTS,
                    .freq = DEFAULT_FREQ
            },
            .valid = true,
        },

        // Only height
        {
            .args = {"./zappy_server", "-y", "15", NULL},
            .expected = {
                    .port = DEFAULT_PORT,
                    .width = DEFAULT_WIDTH,
                    .height = 15,
                    .names = {"Team1", "Team2", "Team3", "Team4", NULL},
                    .slots = DEFAULT_SLOTS,
                    .freq = DEFAULT_FREQ
            },
            .valid = true,
        },

        // Team names
        {
            .args = {"./zappy_server", "-n", "Team1", "Team2", NULL},
            .expected = {
                    .port = DEFAULT_PORT,
                    .width = DEFAULT_WIDTH,
                    .height = DEFAULT_HEIGHT,
                    .names = {"Team1", "Team2", NULL},
                    .slots = DEFAULT_SLOTS,
                    .freq = DEFAULT_FREQ
            },
            .valid = true,
        },

        // No team
        {
            .args = {"./zappy_server", "-n", NULL},
            .valid = false,
        },
        {
            .args = {"./zappy_server", "-n", "-p", "4242", NULL},
            .valid = false,
        },

        // Lot of team
        {
            .args = {"./zappy_server", "-n", "Team1", "Team2", "Team3", "Team4",
                     "Team5", "Team6", "Team7", "Team8", "Team9", "Team10",
                     "Team11", "Team12", "Team13", "Team14", "Team15", "Team16",
                     "Team17", "Team18", "Team19", "Team20", "Team21", "Team22",
                     "Team23", "Team24", "Team25", "Team26", "Team27", "Team28",
                     "Team29", "Team30", "Team31", "Team32", "Team33", "Team34",
                     "Team35", "Team36", "Team37", "Team38", "Team39", "Team40",
                     "Team41", "Team42", "Team43", "Team44", "Team45", "Team46",
                     "Team47", "Team48", "Team49", "Team50", "Team51", "Team52",
                     "Team53", "Team54", "Team55", "Team56", "Team57", "Team58",
                     "Team59", "Team60", "Team61", "Team62", "Team63", "Team64",
                     "Team65", "Team66", "Team67", "Team68", "Team69", "Team70",
                     "Team71", "Team72", "Team73", "Team74", "Team75", "Team76",
                     "Team77", "Team78", "Team79", "Team80", "Team81", "Team82",
                     "Team83", "Team84", "Team85", "Team86", "Team87", "Team88",
                     "Team89", "Team90", "Team91", "Team92", "Team93", "Team94",
                     "Team95", "Team96", "Team97", "Team98", "Team99", "Team100",
                     "Team101", "Team102", "Team103", "Team104", "Team105",
                     "Team106", "Team107", "Team108", "Team109", "Team110",
                     "Team111", "Team112", "Team113", "Team114", "Team115",
                     "Team116", "Team117", "Team118", "Team119", NULL},
            .expected = {
                    .port = DEFAULT_PORT,
                    .width = DEFAULT_WIDTH,
                    .height = DEFAULT_HEIGHT,
                    .names =  {"Team1", "Team2", "Team3", "Team4",
                              "Team5", "Team6", "Team7", "Team8", "Team9", "Team10",
                              "Team11", "Team12", "Team13", "Team14", "Team15", "Team16",
                              "Team17", "Team18", "Team19", "Team20", "Team21", "Team22",
                              "Team23", "Team24", "Team25", "Team26", "Team27", "Team28",
                              "Team29", "Team30", "Team31", "Team32", "Team33", "Team34",
                              "Team35", "Team36", "Team37", "Team38", "Team39", "Team40",
                              "Team41", "Team42", "Team43", "Team44", "Team45", "Team46",
                              "Team47", "Team48", "Team49", "Team50", "Team51", "Team52",
                              "Team53", "Team54", "Team55", "Team56", "Team57", "Team58",
                              "Team59", "Team60", "Team61", "Team62", "Team63", "Team64",
                              "Team65", "Team66", "Team67", "Team68", "Team69", "Team70",
                              "Team71", "Team72", "Team73", "Team74", "Team75", "Team76",
                              "Team77", "Team78", "Team79", "Team80", "Team81", "Team82",
                              "Team83", "Team84", "Team85", "Team86", "Team87", "Team88",
                              "Team89", "Team90", "Team91", "Team92", "Team93", "Team94",
                              "Team95", "Team96", "Team97", "Team98", "Team99", "Team100",
                              "Team101", "Team102", "Team103", "Team104", "Team105",
                              "Team106", "Team107", "Team108", "Team109", "Team110",
                              "Team111", "Team112", "Team113", "Team114", "Team115",
                              "Team116", "Team117", "Team118", "Team119", NULL},
                    .slots = DEFAULT_SLOTS,
                    .freq = DEFAULT_FREQ
            },
            .valid = true,
        },

        // Multiple same team name
        {
            .args = {"./zappy_server", "-n", "Team1", "Team1", NULL},
            .valid = false,
        },
        {
            .args = {"./zappy_server", "-n", "Team1", "Team2", "Team1", NULL},
            .valid = false,
        },
        {
            .args = {"./zappy_server", "-n", "Team1", "Team2", "Team1", "Team2", NULL},
            .valid = false,
        },

        // Too small frequency
        {
            .args = {"./zappy_server", "-f", "0", NULL},
            .valid = false,
        },
        {
            .args = {"./zappy_server", "-f", "-1", NULL},
            .valid = false,
        },

        // Too high frequency
        {
            .args = {"./zappy_server", "-f", "10001", NULL},
            .valid = false,
        },
        {
            .args = {"./zappy_server", "-f", "1000000", NULL},
            .valid = false,
        },
        {
            .args = {"./zappy_server", "-f", "9999", NULL},
            .valid = true,
            .expected = {
                    .port = DEFAULT_PORT,
                    .width = DEFAULT_WIDTH,
                    .height = DEFAULT_HEIGHT,
                    .names = {"Team1", "Team2", "Team3", "Team4", NULL},
                    .slots = DEFAULT_SLOTS,
                    .freq = 9999
            }
        },

        // Too litle slots
        {
            .args = {"./zappy_server", "-c", "0", NULL},
            .valid = false,
        },
        {
            .args = {"./zappy_server", "-c", "-1", NULL},
            .valid = false,
        },

        // End
        {
            .args = {NULL}
        }
};

bool is_args_eq(args_t *args, const args_t_raw *test)
{
    if (args->port != test->port) {
        printf("port: got %d, expected %d\n", args->port, test->port);
        return false;
    }
    if (args->width != test->width) {
        printf("width: got %d, expected %d\n", args->width, test->width);
        return false;
    }
    if (args->height != test->height) {
        printf("height: got %d, expected %d\n", args->height, test->height);
        return false;
    }
    if (args->slots != test->slots) {
        printf("slots: got %d, expected %d\n", args->slots, test->slots);
        return false;
    }
    if (args->freq != test->freq) {
        printf("freq: got %d, expected %d\n", args->freq, test->freq);
        return false;
    }
    if (str_array_len(args->names) != str_array_len((char **)test->names)) {
        printf("names: got %d, expected %d\n", str_array_len(args->names), str_array_len((char **)test->names));
        return false;
    }
    for (int i = 0; test->names[i] != NULL; i++) {
        if (strcmp(args->names[i], test->names[i]) != 0)
            return false;
    }
    return true;
}

Test(get_args, get_args, .init = redirect_all_stdout)
{
    char *err = NULL;

    for (int i = 0; tests[i].args[0] != NULL; i++) {
        optind = 1;
        const arg_test_t *test = &tests[i];
        args_t args = {0};
        bool valid = get_args(str_array_len((char **)test->args), (char **)test->args, &args, &err);

        cr_assert_eq(valid, test->valid, "Test %d: Expected %d, got %d", i + 1, test->valid, valid);
        if (!valid)
            continue;
        cr_assert(is_args_eq(&args, &test->expected), "Test %d: Expected args not equal", i + 1);
    }
}
