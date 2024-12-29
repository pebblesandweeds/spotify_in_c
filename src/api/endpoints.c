#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "api/endpoints.h"
#include "core/http.h"

char *fetch_artist_info(const char *artist_id, const char *token) {
    char url[256];
    snprintf(url, sizeof(url), "https://api.spotify.com/v1/artists/%s", artist_id);

    struct curl_slist *headers = NULL;
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", token);
    headers = curl_slist_append(headers, auth_header);

    if (headers == NULL) {
        fprintf(stderr, "Failed to append authorization header.\n");
        return NULL;
    }

    return perform_http_request(url, NULL, headers);
}

char *fetch_user_playlists(const char *token) {
    // TODO: Implement
    return NULL;
}

char *fetch_playlist_tracks(const char *playlist_id, const char *token) {
    // TODO: Implement
    return NULL;
}
