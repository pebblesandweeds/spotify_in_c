#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/config.h"
#include "api/auth.h"
#include "api/endpoints.h"

int test_api_connection() {
    char *client_id = get_spotify_credentials("CLIENT_ID");
    char *client_secret = get_spotify_credentials("CLIENT_SECRET");
    if (client_id == NULL || client_secret == NULL) {
        fprintf(stderr, "Required Spotify credentials not found in environment.\n");
        return 1;
    }
    char *token = fetch_spotify_token(client_id, client_secret);
    if (token != NULL) {
        printf("Successfully retrieved token: %s\n", token);
        const char *artist_id = "4Z8W4fKeB5YxbusRsdQVPb"; // Radiohead
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

int test_auth_url() {
    char *client_id = get_spotify_credentials("CLIENT_ID");
    char *client_secret = get_spotify_credentials("CLIENT_SECRET");
    if (client_id == NULL || client_secret == NULL) {
        fprintf(stderr, "CLIENT_ID and CLIENT_SECRET not found in environment.\n");
        return 1;
    }

    char *auth_url = generate_auth_url(client_id);
    if (auth_url != NULL) {
        printf("Authorization URL:\n%s\n", auth_url);
        printf("\nVisit this URL in your browser to authorize the application.\n");
        printf("After authorization, paste the full callback URL here: ");

        char callback_url[4096];
        if (fgets(callback_url, sizeof(callback_url), stdin) != NULL) {
            // Remove newline if present
            callback_url[strcspn(callback_url, "\n")] = 0;

            char *token = process_auth_callback_url(callback_url, client_id, client_secret);
            if (token != NULL) {
                printf("Successfully retrieved access token: %s\n", token);
                free_spotify_token(token);
            } else {
                fprintf(stderr, "Failed to process callback URL.\n");
            }
        }

        free(auth_url);
        return 0;
    } else {
        fprintf(stderr, "Failed to generate authorization URL.\n");
        return 1;
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "test") == 0) {
            return test_api_connection();
        } else if (strcmp(argv[1], "auth") == 0) {
            return test_auth_url();
        }
    }
    
    printf("Spotify Playlist Manager\n");
    printf("Commands:\n");
    printf("  test - Test API connectivity\n");
    printf("  auth - Generate authorization URL\n");
    return 0;
}
