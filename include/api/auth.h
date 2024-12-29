#ifndef API_AUTH_H
#define API_AUTH_H

// Authentication handling
char *fetch_spotify_token(const char *client_id, const char *client_secret);
char *refresh_spotify_token(const char *refresh_token);  // NEW: for future use
void free_spotify_token(char *token);  // NEW: explicit cleanup

// Generate authorization URL for user to visit in browser
char *generate_auth_url(const char *client_id);

// Process the full callback URL that user will paste back
// Returns access token that can be used like current token
char *process_auth_callback_url(const char *callback_url,
                              const char *client_id,
                              const char *client_secret);

#endif
