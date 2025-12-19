#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int sock;

/* Receive messages */
void *receive_msg(void *arg) {
    char buffer[BUF_SIZE];
    while (1) {
        int len = recv(sock, buffer, BUF_SIZE, 0);
        if (len > 0) {
            buffer[len] = '\0';
            printf("%s\n", buffer);
        }
    }
}

int main() {
    struct sockaddr_in server;
    char message[BUF_SIZE];
    pthread_t recv_thread;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Connected to chat server\n");

    pthread_create(&recv_thread, NULL, receive_msg, NULL);

    while (1) {
        fgets(message, BUF_SIZE, stdin);
        send(sock, message, strlen(message), 0);
    }

    close(sock);
    return 0;
}
