// udp_calc_client.c
// Compile: gcc udp_calc_client.c -o udp_calc_client
// Run: ./udp_calc_client <server_ip> <server_port>
// Example: ./udp_calc_client 10.0.0.2 8080

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define BUFSZ 1024
#define TIMEOUT_SEC 2      // wait 2 seconds for reply
#define MAX_RETRIES 3

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr,"Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }
    char *srv_ip = argv[1];
    int srv_port = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    struct sockaddr_in server;
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(srv_port);
    if (inet_pton(AF_INET, srv_ip, &server.sin_addr) <= 0) {
        fprintf(stderr, "Invalid server IP\n"); return 1;
    }

    char expr[BUFSZ], buf[BUFSZ];
    printf("Enter expression (e.g., sin(1.57) or 2.5*4) or 'quit':\n");
    while (1) {
        printf("expr> ");
        if (!fgets(expr, sizeof(expr), stdin)) break;
        // remove newline
        size_t L = strlen(expr);
        if (L>0 && expr[L-1]=='\n') expr[L-1]=0;
        if (strcmp(expr,"quit")==0) break;
        if (strlen(expr)==0) continue;

        // send with retry on timeout
        int tries = 0;
        int got_reply = 0;
        while (tries < MAX_RETRIES && !got_reply) {
            ssize_t s = sendto(sock, expr, strlen(expr), 0, (struct sockaddr*)&server, sizeof(server));
            if (s < 0) { perror("sendto"); break; }

            // wait for response with timeout using select
            fd_set rfds; FD_ZERO(&rfds); FD_SET(sock, &rfds);
            struct timeval tv; tv.tv_sec = TIMEOUT_SEC; tv.tv_usec = 0;
            int rv = select(sock+1, &rfds, NULL, NULL, &tv);
            if (rv == -1) { perror("select"); break; }
            else if (rv == 0) {
                // timeout
                tries++;
                printf("No reply (timeout). Retrying %d/%d...\n", tries, MAX_RETRIES);
                continue;
            } else {
                // data available
                socklen_t len = sizeof(server);
                ssize_t r = recvfrom(sock, buf, BUFSZ-1, 0, (struct sockaddr*)&server, &len);
                if (r < 0) { perror("recvfrom"); break; }
                buf[r]=0;
                printf("Result: %s\n", buf);
                got_reply = 1;
            }
        }
        if (!got_reply) {
            printf("ERROR: No response from server after %d retries. Packet lost?\n", MAX_RETRIES);
        }
    }

    close(sock);
    return 0;
}
