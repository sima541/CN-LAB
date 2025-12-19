#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define BUF_SIZE 1024

int main() {
    int sockfd, newfd;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    char buffer[BUF_SIZE], filename[100];
    FILE *fp;
    clock_t start, end;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(9000);

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));
    listen(sockfd, 5);

    printf("Server waiting on port 9000...\n");

    newfd = accept(sockfd, (struct sockaddr *)&client, &len);
    printf("Client connected\n");

    /* RECEIVE COMMAND */
    recv(newfd, buffer, BUF_SIZE, 0);

    if (strcmp(buffer, "DOWNLOAD") == 0) {
        recv(newfd, filename, 100, 0);
        fp = fopen(filename, "r");

        start = clock();
        while (fgets(buffer, BUF_SIZE, fp) != NULL) {
            send(newfd, buffer, BUF_SIZE, 0);
        }
        end = clock();

        printf("Download time (server): %.3f seconds\n",
               (double)(end - start) / CLOCKS_PER_SEC);
        fclose(fp);
    }

    if (strcmp(buffer, "UPLOAD") == 0) {
        recv(newfd, filename, 100, 0);
        fp = fopen(filename, "w");

        start = clock();
        while (recv(newfd, buffer, BUF_SIZE, 0) > 0) {
            fputs(buffer, fp);
        }
        end = clock();

        printf("Upload time (server): %.3f seconds\n",
               (double)(end - start) / CLOCKS_PER_SEC);
        fclose(fp);
    }

    close(newfd);
    close(sockfd);
    return 0;
}
