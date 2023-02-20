#include "buffer-overflow.h"

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
  char buf[CBUFLEN] = {0, };
	int ret, c, port, tmp, len, alen, plen;
	struct sockaddr_in serv_addr;
  uint8_t *pname, *addr, *account, *password;
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
      {"account", required_argument, 0, 'x'},
      {"password", required_argument, 0, 'y'},
      {0, 0, 0, 0}
    };

    const char *opt = "a:p:x:y:0";

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

      case 'x':
        tmp = strlen(optarg);
        alen = tmp;
        account = (uint8_t *)malloc(tmp);
        memcpy(account, optarg, tmp);
        break;

      case 'y':
        tmp = strlen(optarg);
        plen = tmp;
        password = (uint8_t *)malloc(tmp);
        memcpy(password, optarg, tmp);
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

  // Connect to the server
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	
  printf("[*] Connected to %s:%d\n", addr, port);

  // Receive the first message, requiring an account
  ret = read(sock, &len, LBYTES);
  if (ret < 0)
    error_handling("read() error");
  len = ntohs(len);
  printf("[*] Will receive %d bytes", len);
  if (len > CBUFLEN)
    printf(". The buffer overflow will happen");
  printf("\n");

  ret = read(sock, buf, len);
  if (ret < 0)
    error_handling("read() error");
  buf[len] = 0;
  printf("[*] Received %d bytes\n", ret);
  printf("[*] Read the first message: %.*s\n", len, buf);

  // Send the account
  len = htons(alen);
  ret = write(sock, &len, LBYTES);
  if (ret < 0)
    error_handling("write() error");
  len = ntohs(len);
  printf("[*] Will send %d bytes\n", len);

  ret = write(sock, account, alen);
  if (ret < 0)
    error_handling("write() error");
  printf("[*] Sent %d bytes\n", ret);

  // Receive the second message, requiring a password
  memset(buf, 0, CBUFLEN);
  ret = read(sock, &len, LBYTES);
  if (ret < 0)
    error_handling("read() error");
  len = ntohs(len);
   printf("[*] Will receive %d bytes", len);
  if (len > CBUFLEN)
    printf(". The buffer overflow will happen");
  printf("\n");
 
	ret = read(sock, buf, len);
	if (ret < 0)
		error_handling("read() error");
  buf[len] = 0;
  printf("[*] Received %d bytes\n", ret);
	
	printf("[*] Read the second message: %.*s\n", len, buf);

  // Send the password
  len = htons(plen);
  ret = write(sock, &len, LBYTES);
  if (ret < 0)
    error_handling("write() error");
  len = ntohs(len);
  printf("[*] Will send %d bytes\n", len);

  ret = write(sock, password, plen);
  if (ret < 0)
    error_handling("write() error");
  printf("[*] Sent %d bytes\n", ret);

  // Receive the result
  memset(buf, 0, CBUFLEN);
  ret = read(sock, &len, LBYTES);
  if (ret < 0)
    error_handling("read() error");
  len = ntohs(len);
  printf("[*] Will receive %d bytes", len);
  if (len > CBUFLEN)
    printf(". The buffer overflow will happen");
  printf("\n");

  ret = read(sock, buf, len);
  if (ret < 0)
    error_handling("read() error");
  buf[len] = 0;
  printf("[*] Receive %d bytes", ret);

  printf("[*] Result: %.*s\n", len, buf);

	close(sock);
	return 0;
}
