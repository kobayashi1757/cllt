#include <stdio.h>
#include <curl/curl.h>
#include "request.h"
#include "parse.h"
#include "memory.h"

int main(void) {
    CURLcode code = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (code != CURLE_OK) {
        fputs(curl_easy_strerror(code), stderr);
        return 1;
    }

    // char *res = request_detect("Hello world!");
    // if (res == NULL) {
    //     fprintf(stderr, "Empty response");
    //     return 1;
    // }
    // parse_detect(res);
    // xfree(res);

    char *res = request_languages();
    if (res == NULL) {
        fprintf(stderr, "Empty response");
        return 1;
    }
    parse_languages(res);
    xfree(res);

    // char *res = request_translate("Hello world!", "en", "ja");
    // if (res == NULL) {
    //     fprintf(stderr, "Empty response");
    //     return 1;
    // }
    // parse_translate(res);
    // xfree(res);

    // char *res = request_translate_file("test.txt", "en", "ja");
    // if (res == NULL) {
    //     fprintf(stderr, "Empty response");
    //     return 1;
    // }
    // parse_translate_file(res);
    // xfree(res);

    curl_global_cleanup();
    return 0;
}
