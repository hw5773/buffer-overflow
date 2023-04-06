#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <getopt.h>
#include <stdint.h>
#include "vulnerable_server.h"

void usage(const char *pname)
{
  printf(">> Usage: %s [options]\n", pname);
  printf("Options\n");
  printf("  -p, --port     Server's opening port\n");
  exit(0);
}

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock, c, port, tmp, tidx, rc;
  const char *pname;
  uint8_t eflag;

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
      rc = pthread_create(&threads[tidx], &attr, process, &info[tidx]);
      if (rc < 0)
      {
        printf("[*] Error happened in creating a thread\n");
        exit(1);
      }
      tidx++;
    }
  }

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

	close(serv_sock);

	return 0;
}

void *process(void *data)
{
  int i, ret, sock;
  char rbuf[RBUFLEN] = {0, };
  char wbuf[WBUFLEN] = {0, };
  char *p;

  sock = ((info_t *)data)->sock;
  write(sock, 

}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
