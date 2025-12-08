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
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[1024];
    struct Fruit fruits[3] = {{"apple", 10}, {"banana", 8}, {"mango", 5}};
    char customerList[10][50];
    int customerCount = 0;

    // Step 1: Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Step 2: Bind socket to port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    printf("UDP Server started... waiting for clients\n");

    // Step 3: Infinite loop to handle multiple clients
    while (1) {
        addr_size = sizeof(clientAddr);

        // Receive message from client
        recvfrom(sockfd, buffer, sizeof(buffer), 0,
                 (struct sockaddr*)&clientAddr, &addr_size);

        char clientInfo[50];
        sprintf(clientInfo, "%s:%d", inet_ntoa(clientAddr.sin_addr),
                ntohs(clientAddr.sin_port));

        int isNew = 1;
        for (int i = 0; i < customerCount; i++)
            if (strcmp(customerList[i], clientInfo) == 0) isNew = 0;
        if (isNew) strcpy(customerList[customerCount++], clientInfo);

        printf("\nClient %s requested: %s\n", clientInfo, buffer);

        // Parse fruit and quantity
        char fruitName[20];
        int qty, found = 0;
        sscanf(buffer, "%s %d", fruitName, &qty);

        for (int i = 0; i < 3; i++) {
            if (strcmp(fruits[i].name, fruitName) == 0) {
                found = 1;
                if (fruits[i].quantity >= qty) {
                    fruits[i].quantity -= qty;
                    sprintf(buffer, "Sold %d %s(s). Remaining: %d",
                            qty, fruitName, fruits[i].quantity);
                } else {
                    sprintf(buffer, "Sorry, not enough %s in stock!", fruitName);
                }
                break;
            }
        }
        if (!found) strcpy(buffer, "Fruit not found!");

        // Send response back
        sendto(sockfd, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr*)&clientAddr, addr_size);

        printf("Current customers (%d):\n", customerCount);
        for (int i = 0; i < customerCount; i++)
            printf("  %s\n", customerList[i]);
    }

    close(sockfd);
    return 0;
}
