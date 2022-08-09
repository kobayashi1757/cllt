#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "option.h"

Options global_opts;

void parse_arguments(int argc, char **argv) {
    // Parse options
    int opt;
    while ((opt = getopt(argc, argv, ":hlf:o:s:t:")) != -1) {
        switch (opt) {
            case 'h':
                global_opts.help = true;
                break;
            case 'l':
                global_opts.list = true;
                break;
            case 'f':
                global_opts.file = optarg;
                break;
            case 'o':
                global_opts.output = optarg;
                break;
            case 's':
                global_opts.source = optarg;
                break;
            case 't':
                global_opts.target = optarg;
                break;
            default:
                break;
        }
    }

    // The first argument is the text to be translated
    if (optind < argc) {
        global_opts.text = argv[optind];
    }
}

void show_help(void) {
    printf("Usage:\n");
    printf("  cllt [OPTIONS] [TEXT]\n");
    printf("\n");
    printf("Options:\n");
    printf("  -h        : show help\n");
    printf("  -l        : list all supported languages\n");
    printf("  -s <code> : the source language code\n");
    printf("  -t <code> : the target language code\n");
    printf("  -f <file> : the file to be translated\n");
    printf("              if TEXT is given, this option will be ignored\n");
    printf("  -o <file> : the output filename of the translated file\n");
    printf("              if this option is omitted, only a url to the\n");
    printf("              translated file will be printed\n");
}
