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

#define SBUFLEN 16
#define CBUFLEN 256
#define LBYTES 4

void error_handling(char *message);
#endif /* __BUFFER_OVERFLOW_H__ */
