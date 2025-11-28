#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[1024] = "Hi";

    // Step 1: Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("Client socket created successfully.\n");

    // Step 2: Define server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("10.0.0.2"); // Server ka IP (Mininet me h2)

    // Step 3: Connect to server
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(1);
    }
    printf("Connected to server.\n");

    // Step 4: Send message
    send(sockfd, buffer, strlen(buffer) + 1, 0);
    printf("Message sent: %s\n", buffer);

    // Step 5: Receive reply
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server says: %s\n", buffer);

    // Step 6: Close socket
    close(sockfd);
    return 0;
}

