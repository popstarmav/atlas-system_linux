#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define MAX_TODOS 100

typedef struct {
    int id;
    char *title;
    char *description;
} Todo;

Todo todos[MAX_TODOS];
int todo_count = 0;

void send_response(int client_fd, int status_code, const char *content_type, const char *body) {
    char response[BUFFER_SIZE];
    const char *status_text;
    
    switch (status_code) {
        case 201: status_text = "Created"; break;
        case 404: status_text = "Not Found"; break;
        case 411: status_text = "Length Required"; break;
        case 422: status_text = "Unprocessable Entity"; break;
        default: status_text = "OK";
    }

    if (body && content_type) {
        snprintf(response, BUFFER_SIZE,
                "HTTP/1.1 %d %s\r\n"
                "Content-Length: %lu\r\n"
                "Content-Type: %s\r\n"
                "\r\n"
                "%s",
                status_code, status_text, strlen(body), content_type, body);
    } else {
        snprintf(response, BUFFER_SIZE,
                "HTTP/1.1 %d %s\r\n\r\n",
                status_code, status_text);
    }
    
    send(client_fd, response, strlen(response), 0);
}

char *get_header_value(const char *request, const char *header) {
    char *header_start = strstr(request, header);
    if (!header_start) return NULL;
    
    header_start += strlen(header);
    while (*header_start == ' ') header_start++;
    
    char *header_end = strstr(header_start, "\r\n");
    if (!header_end) return NULL;
    
    int length = header_end - header_start;
    char *value = malloc(length + 1);
    strncpy(value, header_start, length);
    value[length] = '\0';
    
    return value;
}

void handle_post_todos(int client_fd, const char *request, const char *body) {
    if (!get_header_value(request, "Content-Length:")) {
        send_response(client_fd, 411, NULL, NULL);
        return;
    }

    char *title = NULL;
    char *description = NULL;
    char *param = strtok(strdup(body), "&");
    
    while (param) {
        char *value = strchr(param, '=');
        if (value) {
            *value = '\0';
            value++;
            if (strcmp(param, "title") == 0) title = strdup(value);
            if (strcmp(param, "description") == 0) description = strdup(value);
        }
        param = strtok(NULL, "&");
    }

    if (!title || !description) {
        send_response(client_fd, 422, NULL, NULL);
        free(title);
        free(description);
        return;
    }

    todos[todo_count].id = todo_count;
    todos[todo_count].title = title;
    todos[todo_count].description = description;

    char json_response[BUFFER_SIZE];
    snprintf(json_response, BUFFER_SIZE,
            "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
            todo_count, title, description);
    
    todo_count++;
    send_response(client_fd, 201, "application/json", json_response);
}

void handle_request(int client_fd, const char *request) {
    char *request_copy = strdup(request);
    char *line = strtok(request_copy, "\r\n");
    char *method = strtok(line, " ");
    char *path = strtok(NULL, " ");
    char *body = strstr(request, "\r\n\r\n");
    
    if (body) body += 4;

    printf("127.0.0.1 %s %s -> ", method, path);

    if (strcmp(path, "/todos") == 0 && strcmp(method, "POST") == 0) {
        handle_post_todos(client_fd, request, body);
        printf("201 Created\n");
    } else {
        send_response(client_fd, 404, NULL, NULL);
        printf("404 Not Found\n");
    }

    free(request_copy);
}

int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    bind(server_fd, (struct sockaddr *)&server, sizeof(server));
    listen(server_fd, 10);

    printf("Server listening on port %d\n", PORT);

    while (1) {
        struct sockaddr_in client;
        socklen_t client_len = sizeof(client);
        int client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);
        
        char buffer[BUFFER_SIZE] = {0};
        recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        
        handle_request(client_fd, buffer);
        close(client_fd);
    }

    return 0;
}
