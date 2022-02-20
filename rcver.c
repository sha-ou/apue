#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "proto.h"

#define IPSTRSIZE 32

int main(int argc, char *argv[])
{
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd < 0) {
        perror("socket");
        exit(1);
    }

    size_t rcvbuflen = sizeof(struct msg_st) + NAMEMAX;
    struct msg_st *rcvbuf = (struct msg_st *)malloc(rcvbuflen);
    if (!rcvbuf) {
        perror("malloc");
        return 1;
    }

    struct sockaddr_in localaddr;
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, "0.0.0.0", &localaddr.sin_addr.s_addr);
    int ret = bind(sfd, (void *)&localaddr, sizeof(localaddr));
    if (ret < 0) {
        close(sfd);
        perror("bind");
        exit(1);
    }

    struct sockaddr_in peeraddr;
    socklen_t paddrlen = sizeof(peeraddr);
    while (1) {
        recvfrom(sfd, rcvbuf, rcvbuflen, 0, (void *)&peeraddr, &paddrlen);
        rcvbuf->name[NAMEMAX - 1] = '\0';

        char addrbuf[IPSTRSIZE] = { 0 };
        inet_ntop(AF_INET, &peeraddr.sin_addr.s_addr, addrbuf, sizeof(addrbuf));
        printf("--- msg from %s:%d ---\n", addrbuf, ntohs(peeraddr.sin_port));
        printf("%8s: %s\n", "name", rcvbuf->name);
        printf("%8s: %u\n", "math", ntohl(rcvbuf->math));
        printf("%8s: %u\n", "chinese", ntohl(rcvbuf->chinese));
    }

    close(sfd);
    free(rcvbuf);
    return 0;
}
