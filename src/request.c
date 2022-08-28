#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "config.h"
#include "memory.h"
#include "request.h"

typedef struct String {
    size_t length;
    char *data;
} String;

typedef struct Postfield {
    const char *key;
    const char *value;
} Postfield;

static size_t string_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
static size_t file_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
static void check_nonnull(void *ptr, const char *function);
static void check_curlcode(CURLcode code, const char *error_buf);
static char *get_urlencoded_postfieids(CURL *curl, int num, Postfield *postfields);
static void handle_error_response(const char *res, long res_code, int num, long *error_codes);

char *request_detect(const char *q) {
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "Something went wrong with curl");
        exit(1);
    }

    String string = { 0 };
    char error_buf[CURL_ERROR_SIZE] = { 0 };
#ifdef API_KEY
    char *postfields = get_urlencoded_postfieids(curl, 2, (Postfield[]) { { "q", q }, { "api_key", API_KEY } });
#else
    char *postfields = get_urlencoded_postfieids(curl, 1, (Postfield[]) { { "q", q } });
#endif

    // Set options
    // TODO: add 'accept: application/json' to the header
    check_curlcode(curl_easy_setopt(curl, CURLOPT_URL, DETECT_URL), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &string), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_callback), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields), NULL);

    // Perform the request
    check_curlcode(curl_easy_perform(curl), error_buf);

    // Check response code
    long response_code;
    check_curlcode(curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code), NULL);
    if (response_code != 200L) {
        handle_error_response(string.data, response_code, 4, (long[]) { 400L, 403L, 429L, 500L });

        // We should never reach here
        assert(false);
    }

    curl_easy_cleanup(curl);
    xfree(postfields);
    return string.data;
}

char *request_languages(void) {
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "Something went wrong with curl");
        exit(1);
    }

    String string = { 0 };
    char error_buf[CURL_ERROR_SIZE] = { 0 };

    // Set options
    // TODO: add 'accept: application/json' to the header
    check_curlcode(curl_easy_setopt(curl, CURLOPT_URL, LANGUAGES_URL), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &string), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_callback), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf), NULL);

    // Perform the request
    check_curlcode(curl_easy_perform(curl), error_buf);

    // Check response code
    long response_code;
    check_curlcode(curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code), NULL);
    if (response_code != 200L) {
        handle_error_response(string.data, response_code, 1, (long[]) { 429L });

        // We should never reach here
        assert(false);
    }

    curl_easy_cleanup(curl);
    return string.data;
}

char *request_translate(const char *q, const char *source, const char *target) {
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "Something went wrong with curl");
        exit(1);
    }

    String string = { 0 };
    char error_buf[CURL_ERROR_SIZE] = { 0 };
#ifdef API_KEY
    char *postfields = get_urlencoded_postfieids(curl, 5, (Postfield[]) { { "q", q }, { "source", source }, { "target", target }, { "format", "text" }, { "api_key", API_KEY } });
#else
    char *postfields = get_urlencoded_postfieids(curl, 4, (Postfield[]) { { "q", q }, { "source", source }, { "target", target }, { "format", "text" } });
#endif

    // Set options
    // TODO: add 'accept: application/json' to the header
    check_curlcode(curl_easy_setopt(curl, CURLOPT_URL, TRANSLATE_URL), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &string), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_callback), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields), NULL);

    // Perform the request
    check_curlcode(curl_easy_perform(curl), error_buf);

    // Check response code
    long response_code;
    check_curlcode(curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code), NULL);
    if (response_code != 200L) {
        handle_error_response(string.data, response_code, 4, (long[]) { 400L, 403L, 429L, 500L });

        // We should never reach here
        assert(false);
    }

    curl_easy_cleanup(curl);
    xfree(postfields);
    return string.data;
}

char *request_translate_file(const char *file, const char *source, const char *target) {
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "Something went wrong with curl");
        exit(1);
    }

    String string = { 0 };
    char error_buf[CURL_ERROR_SIZE] = { 0 };

    // POST form data
    curl_mime *form;
    curl_mimepart *field;
    form = curl_mime_init(curl);
    check_nonnull(form, "curl_mime_init");

    // Add 'file' field
    field = curl_mime_addpart(form);
    check_nonnull(field, "curl_mime_addpart");
    check_curlcode(curl_mime_name(field, "file"), NULL);
    check_curlcode(curl_mime_filedata(field, file), NULL);

    // Add 'source' field
    field = curl_mime_addpart(form);
    check_nonnull(field, "curl_mime_addpart");
    check_curlcode(curl_mime_name(field, "source"), NULL);
    check_curlcode(curl_mime_data(field, source, CURL_ZERO_TERMINATED), NULL);

    // Add 'target' field
    field = curl_mime_addpart(form);
    check_nonnull(field, "curl_mime_addpart");
    check_curlcode(curl_mime_name(field, "target"), NULL);
    check_curlcode(curl_mime_data(field, target, CURL_ZERO_TERMINATED), NULL);

#ifdef API_KEY
    // Add 'api_key' field
    field = curl_mime_addpart(form);
    check_nonnull(field, "curl_mime_addpart");
    check_curlcode(curl_mime_name(field, "api_key"), NULL);
    check_curlcode(curl_mime_data(field, API_KEY, CURL_ZERO_TERMINATED), NULL);
#endif

    // Set options
    // TODO: add 'accept: application/json' to the header
    check_curlcode(curl_easy_setopt(curl, CURLOPT_URL, TRANSLATE_FILE_URL), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &string), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_callback), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_MIMEPOST, form), NULL);

    // Perform the request
    check_curlcode(curl_easy_perform(curl), error_buf);

    // Check response code
    long response_code;
    check_curlcode(curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code), NULL);
    if (response_code != 200L) {
        handle_error_response(string.data, response_code, 4, (long[]) { 400L, 403L, 429L, 500L });

        // We should never reach here
        assert(false);
    }

    curl_easy_cleanup(curl);
    curl_mime_free(form);
    return string.data;
}

void download_file_from_url(const char *url, const char *output) {
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "Something went wrong with curl");
        exit(1);
    }

    FILE *file = fopen(output, "wb");
    if (file == NULL) {
        fprintf(stderr, "Couldn't open \"%s\"", output);
        exit(1);
    }

    char error_buf[CURL_ERROR_SIZE] = { 0 };

    // Set options
    check_curlcode(curl_easy_setopt(curl, CURLOPT_URL, url), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, file_write_callback), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf), NULL);
    check_curlcode(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L), NULL);

    // Perform the request
    check_curlcode(curl_easy_perform(curl), error_buf);

    fclose(file);
    curl_easy_cleanup(curl);
}

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
size_t file_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    FILE **file = userdata;
    return size * fwrite(ptr, size, nmemb, *file);
}

static
void check_nonnull(void *ptr, const char *function) {
    if (ptr == NULL) {
        fprintf(stderr, "Function \"%s\" failed", function);
        exit(1);
    }
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

static
char *get_urlencoded_postfieids(CURL *curl, int num, Postfield *postfields) {
    char *escaped_strings[5];
    size_t lengths[5];
    size_t len = 0;

    // Get encoded strings
    assert(num <= 5 && num > 0);
    for (int i=0; i<num; i++) {
        escaped_strings[i] = curl_easy_escape(curl, postfields[i].value, 0);
        if (escaped_strings[i] == NULL) {
            fprintf(stderr, "URL encoding failed");
            exit(1);
        }

        if (i > 0) {
            len += strlen("&");
        }
        lengths[i] = strlen(postfields[i].key) + strlen("=") + strlen(escaped_strings[i]);
        len += lengths[i];
    }

    // Concatenate all fields
    char *encoded_string = xmalloc(len + 1);
    len = 0;
    for (int i=0; i<num; i++) {
        if (i > 0) {
            snprintf(encoded_string + len, strlen("&") + 1, "&");
            len += strlen("&");
        }
        snprintf(encoded_string + len, lengths[i] + 1, "%s=%s", postfields[i].key, escaped_strings[i]);
        len += lengths[i];
    }

    // Cleanup
    for (int i=0; i<num; i++) {
        curl_free(escaped_strings[i]);
    }
    return encoded_string;
}

static
void handle_error_response(const char *res, long res_code, int num, long *known_error_codes) {
    cJSON *json = NULL;
    cJSON *error = NULL;
    const char *error_string = NULL;

    if (res != NULL) {
        json = cJSON_Parse(res);
    }
    if (json != NULL) {
        error = cJSON_GetObjectItem(json, "error");
    }
    if (error != NULL) {
        error_string = cJSON_GetStringValue(error);
    }

    for (int i=0; i<num; i++) {
        if (known_error_codes[i] == res_code) {
            if (error_string != NULL) {
                fputs(error_string, stderr);
            } else {
                fprintf(stderr, "%ld", res_code);
            }
            exit(1);
        }
    }

    fprintf(stderr, "Unknown response code: %ld", res_code);
    exit(1);
}
