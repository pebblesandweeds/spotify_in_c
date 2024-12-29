#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "api/auth.h"
#include "core/http.h"

static char *extract_access_token(const char *json_response) {
    const char *access_token_start = strstr(json_response, "\"access_token\":\"");
    if (access_token_start == NULL) {
        return NULL;
    }
    access_token_start += strlen("\"access_token\":\"");
    
    const char *access_token_end = strchr(access_token_start, '"');
    if (access_token_end == NULL) {
        return NULL;
    }
    
    size_t access_token_length = access_token_end - access_token_start;
    char *access_token = (char *)malloc(access_token_length + 1);
    if (access_token == NULL) {
        return NULL;
    }
    
    strncpy(access_token, access_token_start, access_token_length);
    access_token[access_token_length] = '\0';
    return access_token;
}

char *fetch_spotify_token(const char *client_id, const char *client_secret) {
    char postfields[256];
    snprintf(postfields, sizeof(postfields), 
             "grant_type=client_credentials&client_id=%s&client_secret=%s", 
             client_id, client_secret);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    char *token_response = perform_http_request(
        "https://accounts.spotify.com/api/token", 
        postfields, 
        headers
    );

    if (token_response == NULL) {
        return NULL;
    }

    char *access_token = extract_access_token(token_response);
    free(token_response);
    return access_token;
}

void free_spotify_token(char *token) {
    if (token) {
        free(token);
    }
}

char *generate_auth_url(const char *client_id) {
    const char *base_url = "https://accounts.spotify.com/authorize";
    const char *redirect_uri = "http://localhost:3000";
    const char *response_type = "code";
    const char *scope = "playlist-read-private playlist-read-collaborative";

    // Calculate required buffer size for URL
    size_t url_size = strlen(base_url) +
                     strlen("?client_id=") + strlen(client_id) +
                     strlen("&response_type=") + strlen(response_type) +
                     strlen("&redirect_uri=") + strlen(redirect_uri) +
                     strlen("&scope=") + strlen(scope) + 1;

    char *auth_url = (char *)malloc(url_size);
    if (auth_url == NULL) {
        return NULL;
    }

    // Build URL with proper parameters
    snprintf(auth_url, url_size,
             "%s?client_id=%s&response_type=%s&redirect_uri=%s&scope=%s",
             base_url, client_id, response_type, redirect_uri, scope);

    return auth_url;
}

static char *base64_encode(const char *input) {
    static const char base64_chars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    size_t input_len = strlen(input);
    size_t output_len = 4 * ((input_len + 2) / 3);

    char *encoded = (char *)malloc(output_len + 1);
    if (encoded == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    while (i < input_len) {
        uint32_t octet_a = i < input_len ? (unsigned char)input[i++] : 0;
        uint32_t octet_b = i < input_len ? (unsigned char)input[i++] : 0;
        uint32_t octet_c = i < input_len ? (unsigned char)input[i++] : 0;

        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        encoded[j++] = base64_chars[(triple >> 18) & 0x3F];
        encoded[j++] = base64_chars[(triple >> 12) & 0x3F];
        encoded[j++] = base64_chars[(triple >> 6) & 0x3F];
        encoded[j++] = base64_chars[triple & 0x3F];
    }

    if (input_len % 3 >= 1) encoded[output_len - 1] = '=';
    if (input_len % 3 == 1) encoded[output_len - 2] = '=';

    encoded[output_len] = '\0';
    return encoded;
}

static char *extract_auth_code(const char *callback_url) {
    const char *code_start = strstr(callback_url, "code=");
    if (code_start == NULL) {
        return NULL;
    }
    code_start += 5; // Length of "code="

    const char *code_end = strchr(code_start, '&');
    if (code_end == NULL) {
        code_end = code_start + strlen(code_start);
    }

    size_t code_length = code_end - code_start;
    char *code = (char *)malloc(code_length + 1);
    if (code == NULL) {
        return NULL;
    }

    strncpy(code, code_start, code_length);
    code[code_length] = '\0';

    return code;
}

char *process_auth_callback_url(const char *callback_url,
                             const char *client_id,
                             const char *client_secret) {
   char *auth_code = extract_auth_code(callback_url);
   if (auth_code == NULL) {
       fprintf(stderr, "Failed to extract auth code\n");
       return NULL;
   }

   char *redirect_uri = "http://localhost:3000";
   char postfields[4096];
   snprintf(postfields, sizeof(postfields),
            "grant_type=authorization_code&"
            "code=%s&"
            "redirect_uri=%s&"
            "client_id=%s&"
            "client_secret=%s",
            auth_code, redirect_uri, client_id, client_secret);

   struct curl_slist *headers = NULL;
   headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

   printf("Making request to token endpoint:\n");
   printf("Post fields:\n%s\n", postfields);

   char *response = perform_http_request(
       "https://accounts.spotify.com/api/token",
       postfields,
       headers
   );

   free(auth_code);

   if (response != NULL) {
       printf("Response from Spotify:\n%s\n", response);
       char *access_token = extract_access_token(response);
       free(response);
       return access_token;
   }

   fprintf(stderr, "No response from token endpoint\n");
   return NULL;
}
