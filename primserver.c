#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "calcthread.h"
#define PORT 4020
#define BACKLOG 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    int opt = 1;
    char server_ip_adress[] = "127.0.0.1";
    char hello_message[] = "What number do you want to test?\n";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt SO_REUSEADDR failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(server_ip_adress);
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, addrlen) == -1) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        printf("Waiting for a new connection...\n");

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) == -1) {
            perror("accept failed");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Connection accepted from %s:%d\n", client_ip, ntohs(address.sin_port));

        if (send(new_socket, hello_message, strlen(hello_message), 0) == -1) {
            perror("send failed");
        } else {
            printf("Sent message to client.\n");
        }

        ssize_t bytes_received = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received == -1) {
            perror("recv failed");
        } else if (bytes_received == 0) {
            printf("Client disconnected.\n");
        } else {
            buffer[bytes_received] = '\0';
            printf("Received from client: %s\n", buffer);
        }
        pthread_t new_thread; 
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        int *socket_for_thread = malloc(sizeof(int));
        *socket_for_thread = new_socket;
       

        struct routine_args_t *routine_args = malloc(sizeof(routine_args));
        routine_args->socket = new_socket;
        routine_args->number = atoi(buffer);
        printf("maint thread: number-> %d\n", routine_args->number);
        if(pthread_create(&new_thread, &attr, routine ,(void*) routine_args) != 0) 
        {
            perror("Thread creation failed.");
        }
   }

    close(server_fd);
    printf("Server shut down.\n");

    return 0;
}

