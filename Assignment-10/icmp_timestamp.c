#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;

    if (len == 1)
        sum += *(unsigned char*)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int main() {
    int sock;
    struct sockaddr_in dest;
    char packet[64];

    struct icmphdr *icmp = (struct icmphdr *) packet;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr("10.0.0.2"); // target

    icmp->type = ICMP_TIMESTAMP;
    icmp->code = 0;
    icmp->checksum = 0;
    icmp->un.echo.id = getpid();
    icmp->un.echo.sequence = 1;
    icmp->checksum = checksum(packet, sizeof(packet));

    sendto(sock, packet, sizeof(packet), 0,
           (struct sockaddr *)&dest, sizeof(dest));

    printf("ICMP Timestamp request sent\n");
    close(sock);
    return 0;
}
