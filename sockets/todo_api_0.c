#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 4096

const char *response = "HTTP/1.1 200 OK\r\n\r\n";

void parse_request_line(char *request_line) {
    char *method = strtok(request_line, " ");
    char *path = strtok(NULL, " ");
    char *version = strtok(NULL, "\r\n");
    
    printf("Method: %s\n", method);
    printf("Path: %s\n", path);
    printf("Version: %s\n", version);
}

int main(void) {
    int server_fd, client_fd;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        printf("Client connected: %s\n", inet_ntoa(client.sin_addr));

        ssize_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Raw request: \"%s\"\n", buffer);

            char *request_line = strdup(buffer);
            if (request_line) {
                char *end = strchr(request_line, '\r');
                if (end) *end = '\0';
                parse_request_line(request_line);
                free(request_line);
            }

            send(client_fd, response, strlen(response), 0);
        }

        close(client_fd);
    }

    close(server_fd);
    return EXIT_SUCCESS;
}

