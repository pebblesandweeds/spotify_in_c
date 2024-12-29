#ifndef API_ENDPOINTS_H
#define API_ENDPOINTS_H

char *fetch_artist_info(const char *artist_id, const char *token);
char *fetch_user_playlists(const char *token);
char *fetch_playlist_tracks(const char *playlist_id, const char *token);

#endif
