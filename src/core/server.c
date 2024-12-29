#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#define PORT 3000
#define BUFFER_SIZE 1024

static char *extract_auth_code(const char *request) {
    const char *code_start = strstr(request, "code=");
    if (!code_start) return NULL;
    
    code_start += 5; // Skip "code="
    char *code_end = strchr(code_start, ' ');
    if (!code_end) code_end = strchr(code_start, '\r');
    if (!code_end) return NULL;

    size_t code_len = code_end - code_start;
    char *code = malloc(code_len + 1);
    if (!code) return NULL;

    strncpy(code, code_start, code_len);
    code[code_len] = '\0';
    return code;
}

char *start_oauth_server(void) {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *auth_code = NULL;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        return NULL;
    }

    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        return NULL;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return NULL;
    }

    // Listen for connections
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        return NULL;
    }

    printf("Waiting for Spotify callback on http://localhost:%d\n", PORT);

    // Accept connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        return NULL;
    }

    // Read request
    read(client_fd, buffer, BUFFER_SIZE);

    // Extract auth code
    auth_code = extract_auth_code(buffer);

    // Send response to browser
    const char *response = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n\r\n"
                          "<html><body><h1>Authorization successful!</h1>"
                          "<p>You can close this window now.</p></body></html>";
    write(client_fd, response, strlen(response));

    // Cleanup
    close(client_fd);
    close(server_fd);

    return auth_code;
}
