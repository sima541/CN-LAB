#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server;
    char buffer[BUF_SIZE], filename[100];
    FILE *fp;
    clock_t start, end;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    server.sin_addr.s_addr = inet_addr("10.0.0.1"); // h1 IP

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));

    /* DOWNLOAD */
    strcpy(buffer, "DOWNLOAD");
    send(sockfd, buffer, BUF_SIZE, 0);

    strcpy(filename, "server_file.txt");
    send(sockfd, filename, 100, 0);

    fp = fopen("downloaded_from_server.txt", "w");

    start = clock();
    while (recv(sockfd, buffer, BUF_SIZE, 0) > 0) {
        fputs(buffer, fp);
    }
    end = clock();

    printf("Download time (client): %.3f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    fclose(fp);
    close(sockfd);

    /* UPLOAD */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(sockfd, (struct sockaddr *)&server, sizeof(server));

    strcpy(buffer, "UPLOAD");
    send(sockfd, buffer, BUF_SIZE, 0);

    strcpy(filename, "client_file.txt");
    send(sockfd, filename, 100, 0);

    fp = fopen(filename, "r");

    start = clock();
    while (fgets(buffer, BUF_SIZE, fp) != NULL) {
        send(sockfd, buffer, BUF_SIZE, 0);
    }
    end = clock();

    printf("Upload time (client): %.3f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    fclose(fp);
    close(sockfd);

    return 0;
}
