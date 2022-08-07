#include <stdio.h>
#include <curl/curl.h>
#include "option.h"
#include "request.h"
#include "parse.h"
#include "memory.h"

int main(int argc, char **argv) {
    CURLcode code = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (code != CURLE_OK) {
        fputs(curl_easy_strerror(code), stderr);
        return 1;
    }

    parse_arguments(argc, argv);

    if (global_opts.help) {

        // Show the usage of this program
        show_help();

    } else if (global_opts.list) {

        // List all supported languages
        char *res = request_languages();
        if (res == NULL) {
            fprintf(stderr, "Empty response");
            return 1;
        }
        parse_languages(res);
        xfree(res);

    } else if (global_opts.text != NULL) {

        if (global_opts.source == NULL && global_opts.target == NULL) {
            // Detect the language of the given text
            char *res = request_detect(global_opts.text);
            if (res == NULL) {
                fprintf(stderr, "Empty response");
                return 1;
            }
            parse_detect(res);
            xfree(res);
        } else {
            // Translate the given text
            if (global_opts.source == NULL || global_opts.target == NULL) {
                fprintf(stderr, "Please set both the source language and the target language");
                return 1;
            }
            char *res = request_translate(global_opts.text, global_opts.source, global_opts.target);
            if (res == NULL) {
                fprintf(stderr, "Empty response");
                return 1;
            }
            parse_translate(res);
            xfree(res);
        }

    } else if (global_opts.file != NULL) {

        // Translate the given file
        if (global_opts.source == NULL || global_opts.target == NULL) {
            fprintf(stderr, "Please set both the source language and the target language");
            return 1;
        }
        char *res = request_translate_file(global_opts.file, global_opts.source, global_opts.target);
        if (res == NULL) {
            fprintf(stderr, "Empty response");
            return 1;
        }
        parse_translate_file(res);
        xfree(res);

    } else {

        // Show some help when we don't know how to process the arguments
        show_help();

    }

    curl_global_cleanup();
    return 0;
}
