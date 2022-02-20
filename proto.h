#ifndef PROTO_H__
#define PROTO_H__

#include <stdint.h>

#define MTGRP "224.2.2.2"
#define MTGRP1 "224.2.2.3"
#define RCVPORT "9527"
#define NAMEMAX (512 - 8 - 8)

struct msg_st {
    uint32_t math;
    uint32_t chinese;
    uint8_t name[0];
} __attribute__((packed));

#endif
