#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(void)
{
    int server_fd;
    struct sockaddr_in address;

    /* Create a socket file descriptor */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket failed");
        return (1);
    }

    /* Set up the server address */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(12345);

    /* Bind the socket to the server address */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("bind failed\n");
        return (1);
    }

    /* Listen for incoming connections */
    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        return (1);
    }

    printf("Server listening on port 12345...\n");

    while (1)
    {
        sleep(1);
    }

    return (0);
}
