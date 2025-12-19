// pcap_parser.c
// Compile: gcc pcap_parser.c -o pcap_parser -lpcap
#include <pcap.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc != 2) { fprintf(stderr,"Usage: %s file.pcap\n", argv[0]); return 1; }
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline(argv[1], errbuf);
    if (!pcap) { fprintf(stderr,"pcap_open_offline failed: %s\n", errbuf); return 1; }

    struct pcap_pkthdr *hdr;
    const u_char *pkt;
    int res;
    while ((res = pcap_next_ex(pcap, &hdr, &pkt)) >= 0) {
        if (res == 0) continue; // timeout (not in offline but keep)
        double ts = hdr->ts.tv_sec + hdr->ts.tv_usec/1e6;
        // Ethernet header
        struct ether_header *eth = (struct ether_header*)pkt;
        uint16_t eth_type = ntohs(eth->ether_type);
        if (eth_type == ETHERTYPE_IP) {
            struct ip *ip_hdr = (struct ip*)(pkt + sizeof(struct ether_header));
            char src[INET_ADDRSTRLEN], dst[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ip_hdr->ip_src), src, sizeof(src));
            inet_ntop(AF_INET, &(ip_hdr->ip_dst), dst, sizeof(dst));
            printf("%.6f IP %s -> %s proto=%d\n", ts, src, dst, ip_hdr->ip_p);
            if (ip_hdr->ip_p == IPPROTO_ICMP) {
                struct icmphdr *ic = (struct icmphdr*)(pkt + sizeof(struct ether_header) + ip_hdr->ip_hl*4);
                printf("   ICMP type=%d code=%d\n", ic->type, ic->code);
            } else if (ip_hdr->ip_p == IPPROTO_TCP) {
                printf("   TCP packet\n");
            } else if (ip_hdr->ip_p == IPPROTO_UDP) {
                printf("   UDP packet\n");
            }
        } else {
            printf("%.6f NON-IP ethertype=0x%04x\n", ts, eth_type);
        }
    }
    pcap_close(pcap);
    return 0;
}
