#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

unsigned short checksum(unsigned short *ptr,int nbytes) {
    long sum;
    unsigned short oddbyte;
    short answer;

    sum = 0;
    while(nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if(nbytes == 1) {
        oddbyte = 0;
        *((u_char*)&oddbyte) = *(u_char*)ptr;
        sum += oddbyte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = (short)~sum;
    return answer;
}

int main() {
    int sock;
    char packet[4096];

    struct iphdr *iph = (struct iphdr *) packet;
    struct tcphdr *tcph = (struct tcphdr *) (packet + sizeof(struct iphdr));
    char *data = packet + sizeof(struct iphdr) + sizeof(struct tcphdr);

    strcpy(data, "CSM24037");   // 👈 apna roll number

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr("10.0.0.2"); // target IP

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(data);
    iph->id = htons(54321);
    iph->ttl = 64;
    iph->protocol = IPPROTO_TCP;
    iph->saddr = inet_addr("10.0.0.1");
    iph->daddr = dest.sin_addr.s_addr;
    iph->check = checksum((unsigned short *)packet, iph->tot_len);

    tcph->source = htons(1234);
    tcph->dest = htons(80);
    tcph->seq = 0;
    tcph->doff = 5;
    tcph->syn = 1;
    tcph->window = htons(5840);
    tcph->check = 0;

    sendto(sock, packet, iph->tot_len, 0,
           (struct sockaddr *)&dest, sizeof(dest));

    printf("RAW TCP packet sent with roll number payload\n");
    close(sock);
    return 0;
}
