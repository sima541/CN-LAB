#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    char buffer[1024], fruitName[20];
    int qty;

    // Step 1: Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Step 2: Define server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("10.0.0.2"); // Server IP (h2)

    addr_size = sizeof(serverAddr);

    // Step 3: Send fruit request
    printf("Enter fruit name and quantity (e.g., apple 2): ");
    scanf("%s %d", fruitName, &qty);
    sprintf(buffer, "%s %d", fruitName, qty);

    sendto(sockfd, buffer, strlen(buffer) + 1, 0,
           (struct sockaddr*)&serverAddr, addr_size);

    // Step 4: Receive response from server
    recvfrom(sockfd, buffer, sizeof(buffer), 0,
             (struct sockaddr*)&serverAddr, &addr_size);

    printf("Server reply: %s\n", buffer);

    close(sockfd);
    return 0;
}
