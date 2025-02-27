#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in server;
	struct hostent *host;
	unsigned short port;
	
	if (argc != 3)
	{
		printf("Usage: %s <host> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	/* Get host */
	host = gethostbyname(argv[1]);
	if (host == NULL)
	{
		herror("gethostbyname");
		exit(EXIT_FAILURE);
	}
	
	/* Convert port from string to number */
	port = (unsigned short)atoi(argv[2]);
	
	/* Prepare the server address structure */
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	memcpy(&server.sin_addr, host->h_addr_list[0], host->h_length);
	
	/* Connect to server */
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}
	
	printf("Connected to %s:%d\n", argv[1], port);
	
	close(sock);
	return (EXIT_SUCCESS);
}

