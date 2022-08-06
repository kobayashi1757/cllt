#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>
#include "parse.h"

void parse_detect(const char *response) {
    cJSON *json, *detect, *language;
    const char *language_string;

    json = cJSON_Parse(response);
    if (json == NULL) {
        goto fail;
    }
    detect = cJSON_GetArrayItem(json, 0);
    if (detect == NULL) {
        goto fail;
    }
    language = cJSON_GetObjectItem(detect, "language");
    if (language == NULL) {
        goto fail;
    }
    language_string = cJSON_GetStringValue(language);
    if (language_string == NULL) {
        goto fail;
    }

    fputs(language_string, stdout);

    cJSON_Delete(json);
    return;

fail:
    fprintf(stderr, "Parsing failed");
    exit(1);
}

void parse_languages(const char *response) {
    cJSON *json, *language, *code, *name;
    const char *code_string, *name_string;

    json = cJSON_Parse(response);
    if (json == NULL) {
        goto fail;
    }

    cJSON_ArrayForEach(language, json) {
        code = cJSON_GetObjectItem(language, "code");
        name = cJSON_GetObjectItem(language, "name");
        if (code == NULL || name == NULL) {
            goto fail;
        }
        code_string = cJSON_GetStringValue(code);
        name_string = cJSON_GetStringValue(name);
        if (code_string == NULL || name_string == NULL) {
            goto fail;
        }
        printf("%s: %s\n", code_string, name_string);
    }

    cJSON_Delete(json);
    return;

fail:
    fprintf(stderr, "Parsing failed");
    exit(1);
}

void parse_translate(const char *response) {
    cJSON *json, *translated_text;
    const char *translated_text_string;

    json = cJSON_Parse(response);
    if (json == NULL) {
        goto fail;
    }
    translated_text = cJSON_GetObjectItem(json, "translatedText");
    if (translated_text == NULL) {
        goto fail;
    }
    translated_text_string = cJSON_GetStringValue(translated_text);
    if (translated_text_string == NULL) {
        goto fail;
    }

    fputs(translated_text_string, stdout);

    cJSON_Delete(json);
    return;

fail:
    fprintf(stderr, "Parsing failed");
    exit(1);
}

void parse_translate_file(const char *response) {
    cJSON *json, *translated_file_url;
    const char *translated_file_url_string;

    json = cJSON_Parse(response);
    if (json == NULL) {
        goto fail;
    }
    translated_file_url = cJSON_GetObjectItem(json, "translatedFileUrl");
    if (translated_file_url == NULL) {
        goto fail;
    }
    translated_file_url_string = cJSON_GetStringValue(translated_file_url);
    if (translated_file_url_string == NULL) {
        goto fail;
    }

    fputs(translated_file_url_string, stdout);

    cJSON_Delete(json);
    return;

fail:
    fprintf(stderr, "Parsing failed");
    exit(1);
}
