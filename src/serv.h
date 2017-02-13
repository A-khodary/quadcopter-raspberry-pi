#ifndef SERV_H_
#define SERV_H_

/*
#ifdef __cplusplus
extern "C" {
#endif
*/


#include "network.h"

typedef struct args_SERVER {
	char verbose;
	int sock;
	PMutex * pmutexRemoteConnect;
	DataController * dataController;

} args_SERVER;

int initArgServ(args_SERVER ** argServ,char verbose);

void clean_args_SERVER(args_SERVER * arg);

void *thread_TCP_SERVER(void *args);
void *thread_UDP_SERVER(void *args);

/*
#ifdef __cplusplus
}
#endif
*/
#endif /* SERV_H_ */
