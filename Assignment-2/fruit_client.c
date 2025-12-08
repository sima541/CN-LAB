#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[1024], fruitName[20];
    int qty;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("10.0.0.2");

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connect error");
        exit(1);
    }

    printf("Enter fruit name and quantity (e.g., apple 2): ");
    scanf("%s %d", fruitName, &qty);
    sprintf(buffer, "%s %d", fruitName, qty);

    send(sockfd, buffer, strlen(buffer) + 1, 0);
    recv(sockfd, buffer, sizeof(buffer), 0);

    printf("Server reply: %s\n", buffer);

    close(sockfd);
    return 0;
}
