#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <getopt.h>
#include <stdint.h>

void error_handling(char *message);

void usage(const char *pname)
{
  printf(">> Usage: %s [options]\n", pname);
  printf("Options\n");
  printf("  -a, --addr       Server's address\n");
  printf("  -p, --port       Server's port\n");
  exit(0);
}

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
  char buf[128] = {0, };
	char message[30] = {0, };
	int c, port, tmp, str_len;
  uint8_t *pname, *addr;
  uint8_t eflag;

  pname = argv[0];
  addr = NULL;
  port = -1;
  eflag = 0;

  while (1)
  {
    int option_index = 0;
    static struct option long_options[] = {
      {"addr", required_argument, 0, 'a'},
      {"port", required_argument, 0, 'p'},
      {0, 0, 0, 0}
    };

    const char *opt = "a:p:0";

    c = getopt_long(argc, argv, opt, long_options, &option_index);

    if (c == -1)
      break;

    switch (c)
    {
      case 'a':
        tmp = strlen(optarg);
        addr = (uint8_t *)malloc(tmp);
        memcpy(addr, optarg, tmp);
        break;

      case 'p':
        port = atoi(optarg);
        break;

      default:
        usage(pname);
    }
  }

  if (!addr)
  {
    printf("[*] Please specify the server's address to connect\n");
    eflag = 1;
  }

  if (port < 0)
  {
    printf("[*] Please specify the server's port to connect\n");
    eflag = 1;
  }

  if (eflag)
  {
    usage(pname);
    exit(0);
  }

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(addr);
	serv_addr.sin_port = htons(port);

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	
  printf("[*] Connected to %s:%d\n", addr, port);
  str_len = read(sock, message, sizeof(message));
  if (str_len == -1)
    error_handling("read() error");

  printf("[*] Read the first message: %.*s\n", str_len, message);
  scanf("%s", buf);
  str_len = write(sock, buf, strlen(buf));
  if (str_len == -1)
    error_handling("write() error");

  memset(message, 0, sizeof(message));
	str_len = read(sock, message, sizeof(message));
	if (str_len == -1)
		error_handling("read() error");
	
	printf("[*] Read the second message: %.*s\n", str_len, message);
  memset(buf, 0, sizeof(buf));
  scanf("%s", buf);

  str_len = write(sock, buf, strlen(buf));
  if (str_len == -1)
    error_handling("write() error");
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
