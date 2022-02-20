#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <net/if.h>
#include "proto.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage...\n");
        return 1;
    }

    size_t namelen = strlen(argv[1]);
    if (namelen > NAMEMAX - 1) {
        fprintf(stderr, "name too long\n");
        return 1;
    }

    size_t sndbuflen = sizeof(struct msg_st) + namelen + 1;
    struct msg_st *sndbuf = (struct msg_st *)malloc(sizeof(*sndbuf) + namelen + 1);
    if (!sndbuf) {
        return 1;
    }

    strcpy((char *)sndbuf->name, argv[1]);
    sndbuf->math = htonl(97);
    sndbuf->chinese = htonl(98);

    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd < 0) {
        perror("socket");
        return 1;
    }

    struct ip_mreqn mreq;
    inet_pton(AF_INET, MTGRP, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("ens33");
    if (setsockopt(sfd, IPPROTO_IP, IP_MULTICAST_IF, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt");
        return 1;
    }

    struct sockaddr_in peeraddr;
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, "127.0.0.1", &peeraddr.sin_addr.s_addr);
    if (sendto(sfd, sndbuf, sndbuflen, 0, (void *)&peeraddr, sizeof(peeraddr)) < 0) {
        perror("sendto");
        close(sfd);
        return 1;
    }

    free(sndbuf);
    close(sfd);

    printf("OK\n");
    return 0;
}
