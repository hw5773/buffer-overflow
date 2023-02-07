#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <getopt.h>
#include <stdint.h>

#define BUFLEN 16

void error_handling(char *message);

void usage(const char *pname)
{
  printf(">> Usage: %s [options]\n", pname);
  printf("Options\n");
  printf("  -p, --port     Server's opening port\n");
  exit(0);
}

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
  const char *pname;
  uint8_t eflag;
  char buf[BUFLEN] = {0, };
  char addr[BUFLEN] = {0, };
  int c, port, tmp, pass;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

  const char *first = "Enter your account: ";
  const char *second = "Enter your password: ";
  const char *answer = "correctpassword";

  port = -1;
  eflag = 0;
  pname = argv[0];
  pass = 0;

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

	if (bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	
	if (listen(serv_sock, 5) == -1)
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
      write(clnt_sock, first, strlen(first));
      read(clnt_sock, buf, sizeof(buf));
      printf("[*] Account from client: %s\n", buf);
      memset(buf, 0, sizeof(buf));
      write(clnt_sock, second, strlen(second));
      read(clnt_sock, buf, sizeof(buf));
      printf("[*] Password from client: %s\n", buf);

      if (!strncmp((const char *)buf, answer, strlen(answer)))
      {
        printf("[*] Password is correct!\n");
        pass = 1;
      }
      else
      {
        printf("[*] Password is incorrect!\n");
      }

      if (pass)
      {
        printf("[*] You get the root privilege!\n");
      }

      printf("[*] Bye-bye!\n");
      memset(buf, 0, sizeof(buf));
      pass = 0;
  	  close(clnt_sock);
    }
  }
	close(serv_sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
