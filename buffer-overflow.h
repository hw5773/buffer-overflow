#ifndef __BUFFER_OVERFLOW_H__
#define __BUFFER_OVERFLOW_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <getopt.h>
#include <stdint.h>
#include <pthread.h>

#define SBUFLEN 16
#define CBUFLEN 256
#define LBYTES 4
#define MAX_THREADS 120

typedef struct info_st {
  int sock;
} info_t;

void error_handling(char *message);
#endif /* __BUFFER_OVERFLOW_H__ */
