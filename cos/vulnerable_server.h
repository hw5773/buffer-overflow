#ifndef __VULN_SERV_H__
#define __VULN_SERV_H__

#define BUFLEN 16
#define RBUFLEN 97
#define WBUFLEN 1024
#define MAX_THREADS 30

void error_handling(char *message);
void *process(void *data);

#endif /* __VULN_SERV_H__ */
