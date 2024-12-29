#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "core/config.h"

char *get_config_value(const char *name) {
    char *value = getenv(name);
    if (value == NULL) {
        fprintf(stderr, "Configuration value %s not set.\n", name);
    }
    return value;
}

char *get_spotify_credentials(const char *cred_type) {
    char env_var[64] = "SPOTIFY_";
    strcat(env_var, cred_type);
    return get_config_value(env_var);
}
