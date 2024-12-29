#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

// Configuration management
char *get_config_value(const char *name);  // More generic name, same functionality for now
char *get_spotify_credentials(const char *cred_type);  // NEW: specifically for Spotify creds

#endif
