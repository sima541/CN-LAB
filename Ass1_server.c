#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[1024];

    // Step 1: Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("Server socket created successfully.\n");

    // Step 2: Assign IP, PORT
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Step 3: Bind socket
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    printf("Bind to port 8080 successful.\n");

    // Step 4: Listen
    listen(sockfd, 5);
    printf("Listening for connections...\n");

    // Step 5: Accept connection
    addr_size = sizeof(clientAddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &addr_size);
    printf("Client connected.\n");

    // Step 6: Receive message from client
    recv(newsockfd, buffer, sizeof(buffer), 0);
    printf("Client says: %s\n", buffer);

    // Step 7: Send reply
    strcpy(buffer, "Hello");
    send(newsockfd, buffer, strlen(buffer) + 1, 0);
    printf("Reply sent to client.\n");

    // Step 8: Close sockets
    close(newsockfd);
    close(sockfd);
    return 0;
}

