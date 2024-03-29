#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <getopt.h>
#include <stdint.h>
#include <pthread.h>
#include "vulnerable_server.h"

typedef struct info_st {
  int sock;
} info_t;

int csock;
int proc();
int shell();
int run();

void usage(const char *pname)
{
  printf(">> Usage: %s [options]\n", pname);
  printf("Options\n");
  printf("  -p, --port     Server's opening port\n");
  exit(0);
}

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock, c, i, port, tmp, tidx, rc;
  const char *pname;
  uint8_t eflag;
  uint8_t addr[INET_ADDRSTRLEN];

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

  pthread_t threads[MAX_THREADS];
  pthread_attr_t attr;
  info_t info[MAX_THREADS];

  port = -1;
  eflag = 0;
  tidx = 0;
  pname = argv[0];

  while (1)
  {
    int option_index = 0;
    static struct option long_options[] = {
      {"port", required_argument, 0, 'p'},
      {0, 0, 0, 0}
    };

    const char *opt = "p:0";

    c = getopt_long(argc, argv, opt, long_options, &option_index);

    if (c == -1)
      break;

    switch (c)
    {
      case 'p':
        port = atoi(optarg);
        break;

      default:
        usage(pname);
    }
  }

  if (port < 0)
  {
    printf("Please specify the server's opening port\n");
    eflag = 1;
  }

  if (eflag)
  {
    usage(pname);
    exit(1);
  }

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	if (bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	
	if (listen(serv_sock, 30) == -1)
		error_handling("listen() error");
	
	clnt_addr_size = sizeof(clnt_addr);

  printf("[*] Server is listening on 0.0.0.0:%d\n", port);
  while (1)
  {
	  clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock)
    {
      inet_ntop(AF_INET, &(clnt_addr.sin_addr), addr, INET_ADDRSTRLEN);
      printf("[*] Accept a client from %s:%d\n", addr, ntohs(clnt_addr.sin_port));

      info[tidx].sock = clnt_sock;
      csock = clnt_sock;
      proc();
      /*
      rc = pthread_create(&threads[tidx], &attr, process, &info[tidx]);
      if (rc < 0)
      {
        printf("[*] Error happened in creating a thread\n");
        exit(1);
      }
      tidx++;
      */
    }
  }

  /*
  for (i=0; i<MAX_THREADS; i++)
  {
    rc = pthread_join(threads[i], NULL);

    if (rc)
    {
      printf("[*] Error happened: return code: %d\n", rc);
      close(serv_sock);
      return 1;
    }
  }
  */
	close(serv_sock);

	return 0;
}

int proc()
{
  int ret;
  ret = run();
  return ret;
}

int shell()
{
  char * const argv[] = {"/bin/sh", NULL};
  const char *info = "You got my shell! Type any command as you want (e.g., touch test)!\n";
  printf("shell()!\n");
  write(csock, info, strlen(info));
  dup2(csock, 0);
  dup2(csock, 1);
  dup2(csock, 2);
  execve("/bin/sh", argv, NULL);

  return 0;
}

int run()
{
  char c;
  int i, j, ret, rlen;
  uint8_t v, tmp;
  uint8_t buf[BUFLEN] = {0, };
  uint8_t rbuf[RBUFLEN] = {0, };
  uint8_t wbuf[WBUFLEN] = {0, };
  uint8_t *p;
  const char *intro = "This is the vulnerable server. The memory addresses of variables are as follows:\n";

  p = wbuf;
  memcpy(p, intro, strlen(intro));
  p += strlen(intro);

  ret = sprintf(p, "main: %p\nshell: %p\nbuf: %p\n", main, shell, buf);
  p += ret;

  printf("main: %p, shell: %p, buf: %p, rbuf: %p, wbuf: %p\n", main, shell, buf, rbuf, wbuf);
  ret = sprintf(p, "Values around buf\n");
  p += ret;

  for (i=0; i<4; i++)
  {
    for (j=0; j<24; j+=8)
    {
      printf("buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x\n", 32*i+j, buf[32*i+j], 32*i+j+1, buf[32*i+j+1], 32*i+j+2, buf[32*i+j+2], 32*i+j+3, buf[32*i+j+3], 32*i+j+4, buf[32*i+j+4], 32*i+j+5, buf[32*i+j+5], 32*i+j+6, buf[32*i+j+6], 32*i+j+7, buf[32*i+j+7]);
      ret = sprintf(p, "buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x\n", 32*i+j, buf[32*i+j], 32*i+j+1, buf[32*i+j+1], 32*i+j+2, buf[32*i+j+2], 32*i+j+3, buf[32*i+j+3], 32*i+j+4, buf[32*i+j+4], 32*i+j+5, buf[32*i+j+5], 32*i+j+6, buf[32*i+j+6], 32*i+j+7, buf[32*i+j+7]);
    p += ret;
    }
  }

  ret = sprintf(p, "Send any message to get my shell (maximum 256 characters): ");
  p += ret;

  write(csock, wbuf, p-wbuf);
  rlen = read(csock, rbuf, RBUFLEN);
  rlen--;

  p = rbuf;
  for (i=0; i<(rlen/2); i+=2)
  {
    v = 0 ;
    p = &(rbuf[i]);
    for (j=0; j<2; j++)
    {
      c = p[j];

      if (c >= '0' && c <= '9')
        tmp = c - '0';
      else if (c >= 'A' && c <= 'F')
        tmp = 10 + c - 'A';
      else if (c >= 'a' && c <= 'f')
        tmp = 10 + c - 'a';
      v = 16 * v + tmp;
    }

    if (i>=0 && i<32)
      buf[i/2] = v;
    else if (i>=32 && i<64)
      buf[i/2 + 16] = v;
    else if (i>=64 && i<96)
      buf[i/2 + 32] = v;
    else if (i>=96 && i<128)
      buf[i/2 + 48] = v;
  }

  for (i=0; i<4; i++)
  {
    for (j=0; j<24; j+=8)
    {
      printf("buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x\n", 32*i+j, buf[32*i+j], 32*i+j+1, buf[32*i+j+1], 32*i+j+2, buf[32*i+j+2], 32*i+j+3, buf[32*i+j+3], 32*i+j+4, buf[32*i+j+4], 32*i+j+5, buf[32*i+j+5], 32*i+j+6, buf[32*i+j+6], 32*i+j+7, buf[32*i+j+7]);
      ret = sprintf(p, "buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x\n", 32*i+j, buf[32*i+j], 32*i+j+1, buf[32*i+j+1], 32*i+j+2, buf[32*i+j+2], 32*i+j+3, buf[32*i+j+3], 32*i+j+4, buf[32*i+j+4], 32*i+j+5, buf[32*i+j+5], 32*i+j+6, buf[32*i+j+6], 32*i+j+7, buf[32*i+j+7]);
    p += ret;
    }
  }

  /*
  if (c)
  {
    for (i=0; i<rlen/2; i+=8)
    {
      printf("c[%d] = %02x, c[%d] = %02x, c[%d] = %02x, c[%d] = %02x, c[%d] = %02x, c[%d] = %02x, c[%d] = %02x, c[%d] = %02x\n", i, c[i], i+1, c[i+1], i+2, c[i+2], i+3, c[i+3], i+4, c[i+4], i+5, c[i+5], i+6, c[i+6], i+7, c[i+7]);
      for (j=0; j<8; j++)
        buf[i+j] = c[i+j];
    }
  }
  else
  {
    printf("convert failed\n");
  }

  printf("before memcpy: c: %p, rlen: %d, rlen/2: %d\n", c, rlen, rlen/2);
  //memcpy(buf, c, rlen/2);
  printf("after memcpy()\n");
  //buf[40] = c[40];
  //buf[41] = c[41];
  //buf[42] = c[42];
  //buf[43] = c[43];
  //buf[44] = c[44];
  //buf[45] = c[45];

  printf("buf: %p, c: %p\n", buf, c);
  for (i=0; i<rlen/2; i++)
    buf[i] = c[i];
  printf("buf: %p, c: %p\n", buf, c);
  printf("buf[42]: %02x, c[42]: %02x\n", buf[42], c[42]);
  printf("buf[43]: %02x, c[43]: %02x\n", buf[43], c[43]);
  printf("buf[44]: %02x, c[44]: %02x\n", buf[44], c[44]);
  printf("buf[45]: %02x, c[45]: %02x\n", buf[45], c[45]);

  free(c);
  */

  /*
  for (i=0; i<48; i+=8)
  {
    printf("buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x, buf[%d] = %02x\n", i, buf[i], i+1, buf[i+1], i+2, buf[i+2], i+3, buf[i+3], i+4, buf[i+4], i+5, buf[i+5], i+6, buf[i+6], i+7, buf[i+7]);
  }
  */

  return 0;
}

/*
void *process(void *data)
{
  int sock;
  sock = ((info_t *)data)->sock;
  run(sock);
  return NULL;
}
*/

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
