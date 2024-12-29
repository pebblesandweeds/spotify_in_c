#ifndef CORE_HTTP_H
#define CORE_HTTP_H

#include <curl/curl.h>

// Internal structure for memory management
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Core HTTP functions
char *perform_http_request(const char *url, const char *postfields, struct curl_slist *headers);

#endif
