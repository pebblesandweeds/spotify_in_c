#include <stdio.h>
#include <string.h>
#include "utils/authentication_check.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "client") == 0) {
            return check_client_auth();
        } else if (strcmp(argv[1], "oauth") == 0) {
            return check_oauth_auth();
        }
    }
    
    printf("Spotify Auth Checker\n");
    printf("Commands:\n");
    printf("  client - Check Client Credentials auth\n");
    printf("  oauth  - Check OAuth Authorization\n");
    return 0;
}
