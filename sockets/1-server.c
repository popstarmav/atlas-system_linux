#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    /* Configure server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    /* Bind socket */
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Failed to bind");
        exit(EXIT_FAILURE);
    }

    /* Listen for connections */
    if (listen(server_fd, 1) < 0)
    {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 12345\n");

    /* Accept connection */
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0)
    {
        perror("Failed to accept connection");
        exit(EXIT_FAILURE);
    }

    /* Print client IP address */
    printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

    /* Close connections */
    close(client_fd);
    close(server_fd);

    return (0);
}

