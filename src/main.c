#include <stdio.h>
#include <curl/curl.h>
#include "request.h"
#include "memory.h"

int main(void) {
    CURLcode code = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (code != CURLE_OK) {
        fputs(curl_easy_strerror(code), stderr);
        return 1;
    }

    char *res = request_detect("Hello world!");
    // char *res = request_languages();
    // char *res = request_translate("Hello world!", "en", "es");
    // char *res = request_translate_file("test.txt", "en", "es");
    fputs((res != NULL) ? res : "Empty response", stdout);
    xfree(res);

    curl_global_cleanup();
    return 0;
}
