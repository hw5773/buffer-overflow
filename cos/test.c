#include <stdio.h>
#include <stdint.h>

int main()
{
  uint8_t *str = "a0";
  int v;
  unsigned char c;
  uint8_t *p;
  
  p = str;
  
  c = *p;
  if (c >= 'a' && c <= 'f')
    v = 10 + c - 'a';
  p++;
  c = *p;
  if (c >= '0' && c <= '9')
    v = v * 16 + c - '0';
  
  printf("v: %x\n", v);

  return 0;
}
