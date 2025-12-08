#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct Fruit {
    char name[20];
    int quantity;
};

int main() {
    int sockfd, newfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[1024];
    struct Fruit fruits[3] = {{"apple", 10}, {"banana", 8}, {"mango", 5}};
    char customerList[10][50];
    int customerCount = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind error");
        exit(1);
    }

    listen(sockfd, 5);
    printf("Server started... waiting for clients\n");

    while (1) {
        addr_size = sizeof(clientAddr);
        newfd = accept(sockfd, (struct sockaddr*)&clientAddr, &addr_size);
        if (newfd < 0) {
            perror("Accept error");
            continue;
        }

        char clientInfo[50];
        sprintf(clientInfo, "%s:%d", inet_ntoa(clientAddr.sin_addr),
                ntohs(clientAddr.sin_port));

        int isNew = 1;
        for (int i = 0; i < customerCount; i++)
            if (strcmp(customerList[i], clientInfo) == 0) isNew = 0;
        if (isNew) strcpy(customerList[customerCount++], clientInfo);

        recv(newfd, buffer, sizeof(buffer), 0);
        printf("\nClient %s requested: %s\n", clientInfo, buffer);

        char fruitName[20];
        int qty, found = 0;
        sscanf(buffer, "%s %d", fruitName, &qty);

        for (int i = 0; i < 3; i++) {
            if (strcmp(fruits[i].name, fruitName) == 0) {
                found = 1;
                if (fruits[i].quantity >= qty) {
                    fruits[i].quantity -= qty;
                    sprintf(buffer,
                            "Sold %d %s(s). Remaining: %d",
                            qty, fruitName, fruits[i].quantity);
                } else {
                    sprintf(buffer, "Sorry, not enough %s in stock!", fruitName);
                }
                break;
            }
        }
        if (!found) strcpy(buffer, "Fruit not found!");

        send(newfd, buffer, strlen(buffer) + 1, 0);

        printf("Current customers (%d):\n", customerCount);
        for (int i = 0; i < customerCount; i++)
            printf("  %s\n", customerList[i]);

        close(newfd);
    }

    close(sockfd);
    return 0;
}
