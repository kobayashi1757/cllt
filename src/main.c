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

    char *langs = request_supported_languages();
    fputs((langs != NULL) ? langs : "Empty response", stdout);
    xfree(langs);

    curl_global_cleanup();
    return 0;
}
