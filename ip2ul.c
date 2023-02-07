#include <stdio.h>
#include <arpa/inet.h>
#include <stdint.h>
#define INVALID 0

unsigned long ipv4_to_ul(const char *ip);

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("error in # of arguments\n");
    return 0;
  }

  unsigned long output = ipv4_to_ul(argv[1]);
  printf("output: %lu\n", output);

  return 0;
}

unsigned long ipv4_to_ul(const char *ip)
{
  unsigned long ret;
  int i;
  const char *start;

  start = ip;
  ret = 0;
  
  for (i=0; i<4; i++)
  {
    char c;
    int n = 0;
    while (1)
    {
      c = *start;
      start++;

      if (c >= '0' && c <= '9')
      {
        n *= 10;
        n += c - '0';
      }
      else if ((i < 3 && c == '.') || i == 3)
      {
        break;
      }
      else
      {
        return INVALID;
      }
    }

    if (n >= 256)
      return INVALID;

    ret <<= 8;
    ret |= n;
  }

  return htonl(ret);
}
