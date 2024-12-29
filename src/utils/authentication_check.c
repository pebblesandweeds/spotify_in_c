#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/core/config.h"
#include "../include/api/auth.h"
#include "../include/api/endpoints.h"
#include "../include/core/server.h"
#include "../include/utils/authentication_check.h"

int check_client_auth(void) {
    char *client_id = get_spotify_credentials("CLIENT_ID");
    char *client_secret = get_spotify_credentials("CLIENT_SECRET");
    if (client_id == NULL || client_secret == NULL) {
        fprintf(stderr, "Required Spotify credentials not found in environment.\n");
        return 1;
    }
    char *token = fetch_spotify_token(client_id, client_secret);
    if (token != NULL) {
        printf("Successfully retrieved token: %s\n", token);
        const char *artist_id = "4Z8W4fKeB5YxbusRsdQVPb"; 
        char *artist_info = fetch_artist_info(artist_id, token);
        if (artist_info != NULL) {
            printf("Artist Info: %s\n", artist_info);
            free(artist_info);
        } else {
            printf("Failed to retrieve artist info.\n");
        }
        free_spotify_token(token);
    } else {
        printf("Failed to retrieve token.\n");
    }
    return 0;
}

int check_oauth_auth(void) {
    char *client_id = get_spotify_credentials("CLIENT_ID");
    char *client_secret = get_spotify_credentials("CLIENT_SECRET");
    if (client_id == NULL || client_secret == NULL) {
        fprintf(stderr, "CLIENT_ID and CLIENT_SECRET not found in environment.\n");
        return 1;
    }
    char *auth_url = generate_auth_url(client_id);
    if (auth_url == NULL) {
        fprintf(stderr, "Failed to generate authorization URL.\n");
        return 1;
    }
    printf("Authorization URL:\n%s\n", auth_url);
    printf("\nOpening browser for authorization...\n");
    char command[4096];
    snprintf(command, sizeof(command), "open '%s'", auth_url);
    system(command);

    char *auth_code = start_oauth_server();
    if (auth_code == NULL) {
        fprintf(stderr, "Failed to get authorization code.\n");
        free(auth_url);
        return 1;
    }

    char callback_url[4096];
    snprintf(callback_url, sizeof(callback_url),
             "http://localhost:3000/?code=%s", auth_code);
    char *token = process_auth_callback_url(callback_url, client_id, client_secret);
    if (token != NULL) {
        printf("Successfully retrieved access token: %s\n", token);
        char *playlists = fetch_user_playlists(token);
        if (playlists != NULL) {
            printf("Your playlists:\n%s\n", playlists);
            free(playlists);
        } else {
            fprintf(stderr, "Failed to fetch playlists.\n");
        }
        free_spotify_token(token);
    } else {
        fprintf(stderr, "Failed to process callback URL.\n");
    }
    free(auth_code);
    free(auth_url);
    return 0;
}
