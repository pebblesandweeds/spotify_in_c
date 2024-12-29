#ifndef API_ENDPOINTS_H
#define API_ENDPOINTS_H

// API endpoints
char *fetch_artist_info(const char *artist_id, const char *token);
// Adding playlist-related functions for future implementation
char *fetch_user_playlists(const char *token);
char *fetch_playlist_tracks(const char *playlist_id, const char *token);

#endif
