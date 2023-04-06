#ifndef __VULN_SERV_H__
#define __VULN_SERV_H__

#define RBUFLEN 16
#define WBUFLEN 256

void error_handling(char *message);
void *process(void *data);

#endif /* __VULN_SERV_H__ */
