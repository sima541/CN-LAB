#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUF_SIZE 1024

int clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* write chat log */
void write_log(char *msg) {
    FILE *fp = fopen("log.txt", "a");
    time_t now = time(NULL);
    fprintf(fp, "[%s] %s\n", ctime(&now), msg);
    fclose(fp);
}

/* send message to all clients except sender */
void broadcast(char *msg, int sender) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++) {
        if (clients[i] != sender) {
            send(clients[i], msg, strlen(msg), 0);
        }
    }
    pthread_mutex_unlock(&lock);
}

/* client thread */
void *client_handler(void *socket_desc) {
    int sock = *(int *)socket_desc;
    free(socket_desc);   // 🔴 IMPORTANT FIX

    char buffer[BUF_SIZE];

    while (1) {
        int read_size = recv(sock, buffer, BUF_SIZE, 0);
        if (read_size <= 0)
            break;

        buffer[read_size] = '\0';

        write_log(buffer);
        broadcast(buffer, sock);
    }

    close(sock);
    pthread_exit(NULL);
}

int main() {
    int sockfd, newfd;
    struct sockaddr_in server, client;
    socklen_t c = sizeof(client);
    pthread_t thread_id;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));
    listen(sockfd, 5);

    printf("Chat Server Started on port 8888...\n");

    while ((newfd = accept(sockfd, (struct sockaddr *)&client, &c))) {
        printf("Client connected\n");

        pthread_mutex_lock(&lock);
        clients[client_count++] = newfd;
        pthread_mutex_unlock(&lock);

        // 🔴 MAIN FIX HERE
        int *new_sock = malloc(sizeof(int));
        *new_sock = newfd;

        pthread_create(&thread_id, NULL, client_handler, (void *)new_sock);
    }

    close(sockfd);
    return 0;
}

