// udp_calc_server.c
// Compile: gcc udp_calc_server.c -o udp_calc_server -lm
// Run: ./udp_calc_server 8080

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define BUFSZ 1024

// trim spaces
void trim(char *s) {
    char *p = s, *q = s;
    while (*p) {
        if (!isspace((unsigned char)*p)) *q++ = *p;
        p++;
    }
    *q = 0;
}

// evaluate simple expression or function
// returns 0 on success, -1 on error; result in res_str
int eval_expression(const char *expr, char *res_str, size_t n) {
    // copy and trim
    char buf[BUFSZ];
    strncpy(buf, expr, BUFSZ-1); buf[BUFSZ-1] = '\0';
    trim(buf);
    if (strlen(buf) == 0) { snprintf(res_str, n, "ERROR: empty expression"); return -1; }

    // check for function form: name(arg) e.g., sin(1.57)
    char *p = strchr(buf, '(');
    if (p) {
        char func[32]; int i=0;
        while (buf[i] && buf[i] != '(' && i < (int)sizeof(func)-1) { func[i]=buf[i]; i++; }
        func[i]=0;
        if (func[0]==0) { snprintf(res_str,n,"ERROR: bad func"); return -1; }
        char *q = strchr(buf, ')');
        if (!q) { snprintf(res_str,n,"ERROR: missing )"); return -1; }
        char argbuf[64]; int len = q - p - 1;
        if (len <= 0 || len >= (int)sizeof(argbuf)) { snprintf(res_str,n,"ERROR: bad arg"); return -1; }
        strncpy(argbuf, p+1, len); argbuf[len]=0;
        char *endptr; double x = strtod(argbuf, &endptr);
        if (endptr == argbuf) { snprintf(res_str,n,"ERROR: bad number"); return -1; }

        double ans;
        if (strcasecmp(func,"sin")==0) ans = sin(x);
        else if (strcasecmp(func,"cos")==0) ans = cos(x);
        else if (strcasecmp(func,"tan")==0) ans = tan(x);
        else if (strcasecmp(func,"sqrt")==0) {
            if (x < 0) { snprintf(res_str,n,"ERROR: sqrt of negative"); return -1; }
            ans = sqrt(x);
        } else if (strcasecmp(func,"log")==0) {
            if (x <= 0) { snprintf(res_str,n,"ERROR: log domain"); return -1; }
            ans = log(x);
        } else if (strcasecmp(func,"exp")==0) ans = exp(x);
        else if (strcasecmp(func,"inv")==0) {
            if (x == 0.0) { snprintf(res_str,n,"ERROR: divide by zero (inv)"); return -1; }
            ans = 1.0 / x;
        } else {
            snprintf(res_str,n,"ERROR: unknown func %s", func);
            return -1;
        }
        snprintf(res_str, n, "%.9g", ans);
        return 0;
    }

    // check for binary ops + - * /
    // find operator (first occurrence of + - * / not at pos 0 for unary)
    char op = 0;
    int pos = -1;
    for (int i=1; buf[i]; ++i) {
        if (buf[i] == '+' || buf[i] == '-' || buf[i] == '*' || buf[i] == '/') { op = buf[i]; pos = i; break; }
    }
    if (op == 0) {
        // maybe a single number
        char *endptr; double v = strtod(buf, &endptr);
        if (endptr != buf) { snprintf(res_str, n, "%.9g", v); return 0; }
        snprintf(res_str,n,"ERROR: cannot parse expression");
        return -1;
    }
    // split operands
    char left[64], right[64];
    int llen = pos;
    int rlen = strlen(buf) - pos - 1;
    if (llen<=0 || rlen<=0 || llen>= (int)sizeof(left) || rlen >= (int)sizeof(right)) {
        snprintf(res_str,n,"ERROR: bad operands"); return -1;
    }
    strncpy(left, buf, llen); left[llen]=0;
    strncpy(right, buf+pos+1, rlen); right[rlen]=0;
    char *end1, *end2;
    double a = strtod(left, &end1);
    double b = strtod(right, &end2);
    if (end1==left || end2==right) { snprintf(res_str,n,"ERROR: bad number"); return -1; }
    double r;
    if (op == '+') r = a + b;
    else if (op == '-') r = a - b;
    else if (op == '*') r = a * b;
    else if (op == '/') {
        if (b == 0.0) { snprintf(res_str,n,"ERROR: division by zero"); return -1; }
        r = a / b;
    } else { snprintf(res_str,n,"ERROR: unsupported op"); return -1; }
    snprintf(res_str, n, "%.9g", r);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }
    int port = atoi(argv[1]);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    struct sockaddr_in server, client;
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("bind"); close(sock); return 1;
    }

    printf("UDP Calculator Server listening on port %d\n", port);

    socklen_t clientlen = sizeof(client);
    char buf[BUFSZ], out[BUFSZ];
    while (1) {
        ssize_t r = recvfrom(sock, buf, BUFSZ-1, 0, (struct sockaddr*)&client, &clientlen);
        if (r < 0) { perror("recvfrom"); continue; }
        buf[r]=0;
        char client_ip[INET_ADDRSTRLEN]; inet_ntop(AF_INET,&client.sin_addr,client_ip,sizeof(client_ip));
        int client_port = ntohs(client.sin_port);
        printf("Request from %s:%d -> '%s'\n", client_ip, client_port, buf);

        int ret = eval_expression(buf, out, sizeof(out));
        if (ret == 0) {
            // normal result
            // optionally include "OK " prefix
            // snprintf(out, sizeof(out), "OK %s", out);
        } else {
            // ret < 0 -> out contains error message already
        }

        ssize_t s = sendto(sock, out, strlen(out), 0, (struct sockaddr*)&client, clientlen);
        if (s < 0) perror("sendto");
    }

    close(sock);
    return 0;
}
