#pragma once
#include <stdbool.h>

typedef struct Options {
    bool help;
    bool list;
    const char *text;
    const char *file;
    const char *output;
    const char *source;
    const char *target;
} Options;

extern Options global_opts;

void parse_arguments(int argc, char **argv);
void show_help(void);
