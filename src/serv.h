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
	int servStop;
	PMutex * pmutexServ;
	volatile sig_atomic_t * boolStopServ;
	PMutex * pmutexRemoteConnect;
	DataController * dataController;

} args_SERVER;

int init_args_SERVER(args_SERVER ** argServ,volatile sig_atomic_t * boolStopServ);
void clean_args_SERVER(args_SERVER * arg);

void set_Serv_Stop(args_SERVER * argServ);
int is_Serv_Stop(args_SERVER * argServ);

void *thread_UDP_SERVER(void *args);

/*
#ifdef __cplusplus
}
#endif
*/
#endif /* SERV_H_ */
