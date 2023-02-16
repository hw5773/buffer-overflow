#include "buffer-overflow.h"

void usage(const char *pname)
{
  printf(">> Usage: %s [options]\n", pname);
  printf("Options\n");
  printf("  -p, --port     Server's opening port\n");
  exit(0);
}

void *process(void *data)
{
  const char *response_header = "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n";
  const char *main = "<html><head><title>Buffer Overflow Test</title></head><body><h1>Buffer Overflow</h1><form><table><tr><td>Enter your account:</td><td><input id='account' type='text'/></td></tr><tr><td>Enter your password:</td><td><input id='password' type='password'/></td></tr><tr><td colspan=2 align='center'><input type='submit'></td></table></form></body></html>";
  const char *second = "Enter your password: ";
  const char *answer = "correctpassword";
  const char *correct = "You are signed in with the root privilege";
  const char *incorrect = "You are not allowed to sign in";

  int pass;
  char buf[SBUFLEN];
  char rbuf[RBUFLEN];
  char wbuf[WBUFLEN];
  int ret, len, sock;

  memset(buf, 0, SBUFLEN);
  pass = 0;

  sock = ((info_t *)data)->sock;

  printf("[%d] Address information\n", sock);
  printf("    main: %p, second: %p, answer: %p\n", main, second, answer);
  printf("    correct: %p, incorrect: %p\n", correct, incorrect);
  printf("    buf: %p, ret: %p, len: %p, pass: %p\n", buf, &ret, &len, &pass);
  printf("    &(buf[0]): %p, &(buf[SBUFLEN]): %p\n", &(buf[0]), &(buf[SBUFLEN]));

  // Receive the HTTP request from a client
  ret = read(sock, rbuf, RBUFLEN);
  rbuf[ret] = 0;
  printf("[%d] Received: %s\n", sock, rbuf);

  // Send the first message requiring an account
  memset(wbuf, 0, WBUFLEN);
  len = sprintf(wbuf, response_header, strlen(main));
  ret = write(sock, wbuf, len);
  if (ret < 0)
    error_handling("write() error");
  printf("[%d] Sent %d bytes\n", sock, ret);
  ret = write(sock, main, strlen(main));
  if (ret < 0)
    error_handling("write() error");
  printf("[%d] Sent %d bytes\n", sock, ret);

  // Process favicon.ico (simply ignore it)
  memset(rbuf, 0, RBUFLEN);
  ret = read(sock, rbuf, RBUFLEN);
  rbuf[ret] = 0;
  printf("[%d] Received: %s\n", sock, rbuf);

  // Receive the accout
  memset(rbuf, 0, RBUFLEN);
  ret = read(sock, rbuf, RBUFLEN);
  if (ret < 0)
    error_handling("read() error");
  rbuf[ret] = 0;
  printf("[%d] Received: %s", sock, rbuf);


  // Print the received account
  printf("[*] Account from client: %s\n", buf);
  memset(buf, 0, SBUFLEN);

  // Send the second message requiring a password
  len = strlen(second);
  len = htons(len);
  ret = write(sock, &len, LBYTES);
  if (ret < 0)
    error_handling("write() error");
  len = ntohs(len);
  printf("[*] Will send %d bytes\n", len);

  ret = write(sock, second, len);
  if (ret < 0)
    error_handling("write() error");
  printf("[*] Sent %d bytes\n", ret);

  // Receive the password
  ret = read(sock, &len, LBYTES);
  if (ret < 0)
    error_handling("read() error");
  len = ntohs(len);
  printf("[*] Will receive %d bytes", len);
  if (len > SBUFLEN)
    printf(". The buffer overflow will happen");
  printf("\n");
  printf("[*] Received %d bytes\n", ret);

  ret = read(sock, buf, len);
  if (ret < 0)
    error_handling("read() error");
  buf[len] = 0;
  printf("[*] Received %d bytes\n", ret);

  // Print the received password
  printf("[*] Password from client: %s\n", buf);

  // Check if the password is correct
  if (!strncmp((const char *)buf, answer, strlen(answer)))
  {
    printf("[*] Password is correct!\n");
    pass = 1;
  }
  else
  {
    printf("[*] Password is incorrect!\n");
  }

  //printf("\npass = %d\n\n", pass);
  if (pass)
  {
    printf("[*] The client is logged in and has the root privilege!\n");

    // Send the result
    len = strlen(correct);
    len = htons(len);
    ret = write(sock, &len, LBYTES);
    if (ret < 0)
      error_handling("write() error");
    len = ntohs(len);

    printf("[*] Will send %d bytes\n", len);
    ret = write(sock, correct, len);
    if (ret < 0)
      error_handling("write() error");
    printf("[*] Sent %d bytes\n", ret);
  }
  else
  {
    // Send the result
    len = strlen(incorrect);
    len = htons(len);
    ret = write(sock, &len, LBYTES);
    if (ret < 0)
      error_handling("write() error");
    len = ntohs(len);

    printf("[*] Will send %d bytes\n", len);
    ret = write(sock, incorrect, len);
    if (ret < 0)
      error_handling("write() error");
    printf("[*] Sent %d bytes\n", ret);
  }

  printf("[*] Bye-bye!\n");
  close(sock);

  return NULL;
}

int main(int argc, char *argv[])
{
  int i, serv_sock, clnt_sock, c, port, tidx, rc;
  const char *pname;
  uint8_t eflag;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

  pthread_t threads[MAX_THREADS];
  pthread_attr_t attr;
  info_t info[MAX_THREADS];

  char addr[SBUFLEN] = {0, };

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
	
	if (listen(serv_sock, MAX_THREADS) == -1)
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
