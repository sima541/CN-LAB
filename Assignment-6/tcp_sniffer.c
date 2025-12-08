// tcp_sniffer.c
// Compile: gcc tcp_sniffer.c -o tcp_sniffer
// Run: sudo ./tcp_sniffer <interface> <max_packets>
// Example: sudo ./tcp_sniffer h1-eth0 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <linux/if_packet.h>
#include <net/ethernet.h> // ETH_P_ALL
#include <net/if.h>
#include <sys/ioctl.h>

#include <netinet/ip.h>   // struct iphdr
#include <netinet/tcp.h>  // struct tcphdr
#include <errno.h>
#include <time.h>

#define BUF_SIZE 65536

void print_mac(unsigned char *mac) {
    printf("%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void print_flags(uint8_t flags) {
    printf("%c", (flags & TH_FIN) ? 'F' : '-');
    printf("%c", (flags & TH_SYN) ? 'S' : '-');
    printf("%c", (flags & TH_RST) ? 'R' : '-');
    printf("%c", (flags & TH_PUSH) ? 'P' : '-');
    printf("%c", (flags & TH_ACK) ? 'A' : '-');
    printf("%c", (flags & TH_URG) ? 'U' : '-');
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <interface> <max_packets>\n", argv[0]);
        return 1;
    }
    char *iface = argv[1];
    int max_packets = atoi(argv[2]);
    if (max_packets <= 0) max_packets = 0; // 0 -> run until ctrl+c

    int rawsock;
    unsigned char *buffer = malloc(BUF_SIZE);
    if (!buffer) { perror("malloc"); return 1; }

    // Create raw packet socket
    rawsock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (rawsock == -1) {
        perror("socket");
        return 1;
    }

    // Bind socket to interface
    struct sockaddr_ll sll;
    struct ifreq ifr;
    memset(&sll, 0, sizeof(sll));
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);
    if (ioctl(rawsock, SIOCGIFINDEX, &ifr) < 0) {
        perror("SIOCGIFINDEX");
        close(rawsock);
        return 1;
    }
    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_ALL);
    if (bind(rawsock, (struct sockaddr *)&sll, sizeof(sll)) == -1) {
        perror("bind");
        close(rawsock);
        return 1;
    }

    printf("Listening on %s ... (capture %s)\n", iface,
           (max_packets>0) ? "limited" : "unlimited");
    printf("Press Ctrl+C to stop\n\n");

    int count = 0;
    while (1) {
        ssize_t numbytes = recvfrom(rawsock, buffer, BUF_SIZE, 0, NULL, NULL);
        if (numbytes < 0) {
            if (errno == EINTR) break;
            perror("recvfrom");
            break;
        }
        count++;
        // Parse Ethernet header
        if (numbytes < sizeof(struct ethhdr)) continue;
        struct ethhdr *eth = (struct ethhdr *)buffer;
        uint16_t ethertype = ntohs(eth->h_proto);

        // Only interested in IPv4
        if (ethertype != ETH_P_IP) continue;

        // Parse IP header
        struct iphdr *ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));
        unsigned int iphdr_len = ip->ihl * 4;
        if (numbytes < sizeof(struct ethhdr) + iphdr_len) continue;

        char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ip->saddr), src_ip, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ip->daddr), dst_ip, INET_ADDRSTRLEN);

        // Only process TCP
        if (ip->protocol != IPPROTO_TCP) continue;
        struct tcphdr *tcp = (struct tcphdr *)(buffer + sizeof(struct ethhdr) + iphdr_len);
        unsigned int tcphdr_len = tcp->doff * 4;

        // Payload calc
        int header_total = sizeof(struct ethhdr) + iphdr_len + tcphdr_len;
        int payload_len = numbytes - header_total;
        if (payload_len < 0) payload_len = 0;

        // Timestamp
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char timestr[64];
        strftime(timestr, sizeof(timestr), "%F %T", t);

        printf("=== Packet #%d @ %s ===\n", count, timestr);
        printf("L2: ");
        print_mac(eth->h_source);
        printf(" -> ");
        print_mac(eth->h_dest);
        printf("   Ethertype: 0x%04x\n", ethertype);

        printf("L3: %s -> %s   Protocol: %d   TTL: %d\n",
               src_ip, dst_ip, ip->protocol, ip->ttl);

        printf("L4: TCP  %u -> %u   Seq: %u  Ack: %u  Win: %u\n",
               ntohs(tcp->source), ntohs(tcp->dest),
               ntohl(tcp->seq), ntohl(tcp->ack_seq), ntohs(tcp->window));

        printf("Flags: ");
        print_flags(((uint8_t*)tcp)[13]);
        printf("   HeaderLen: %u  PayloadLen: %d bytes\n", tcphdr_len, payload_len);

        if (payload_len > 0) {
            int to_print = (payload_len > 32) ? 32 : payload_len;
            unsigned char *payload = buffer + header_total;
            printf("Payload (first %d bytes): ", to_print);
            for (int i = 0; i < to_print; ++i) printf("%02x ", payload[i]);
            printf("\n");
        } else {
            printf("No payload\n");
        }
        printf("\n");

        if (max_packets > 0 && count >= max_packets) break;
    }

    close(rawsock);
    free(buffer);
    return 0;
}
