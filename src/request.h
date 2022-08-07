#pragma once

char *request_detect(const char *q);
char *request_languages(void);
char *request_translate(const char *q, const char *source, const char *target);
char *request_translate_file(const char *file, const char *source, const char *target);

void download_file_from_url(const char *url, const char *output);
