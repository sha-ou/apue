#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "proto.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage...\n");
        return 1;
    }

    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd < 0) {
        perror("socket");
        return 1;
    }

    struct msg_st sndbuf = { 0 };
    snprintf((char *)sndbuf.name, sizeof(sndbuf.name), "%s", "Aya");
    sndbuf.math = ntohl(97);
    sndbuf.chinese = ntohl(98);

    struct sockaddr_in peeraddr;
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, argv[1], &peeraddr.sin_addr.s_addr);
    if (sendto(sfd, &sndbuf, sizeof(sndbuf), 0, (void *)&peeraddr, sizeof(peeraddr)) < 0) {
        perror("sendto");
        close(sfd);
        return 1;
    }

    close(sfd);
    return 0;
}
