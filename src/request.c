#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "memory.h"
#include "request.h"

typedef struct String {
    size_t length;
    char *data;
} String;

static
size_t string_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    String *string = userdata;

    // Reallocate memory and copy data
    string->data = xrealloc(string->data, string->length + total_size + 1);
    strncpy(string->data + string->length, ptr, total_size);

    // Update length infomation and make sure the string is null-terminated
    string->length += total_size;
    string->data[string->length] = '\0';

    return total_size;
}

static
void check_curlcode(CURLcode code, const char *error_buf) {
    if (code != CURLE_OK) {
        if (error_buf != NULL && error_buf[0] != '\0') {
            fputs(error_buf, stderr);
        } else {
            fputs(curl_easy_strerror(code), stderr);
        }
        exit(1);
    }
}

char *request_supported_languages(void) {
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "Something went wrong with curl");
        exit(1);
    }

    String string = { 0 };
    char error_buf[CURL_ERROR_SIZE] = { 0 };

    // Set options
    // TODO: add headers
    check_curlcode(curl_easy_setopt(curl, CURLOPT_URL, "https://libretranslate.com/languages"), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &string), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_callback), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf), NULL);

    // Perform the request
    check_curlcode(curl_easy_perform(curl), error_buf);

    // Check response code
    // TODO: handle error response code
    long response_code;
    check_curlcode(curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code), NULL);
    if (response_code != 200L) {
        fprintf(stderr, "Response code: %ld\n", response_code);
        fprintf(stderr, "Response: %s\n", (string.data != NULL) ? string.data : "(null)");
        exit(1);
    }

    curl_easy_cleanup(curl);
    return string.data;
}
