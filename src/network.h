#ifndef NETWORK_H_
#define NETWORK_H_

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <pthread.h>
#include <sys/select.h>

#define SIZE_SOCKET_MESSAGE 100

char get_IP_Port(char *message,struct sockaddr_in * sa);

char isMessageSTOP(char * message);

char isMessagePause(char * message);

int bindUDPSock(int * sock, struct sockaddr_in * adr_svr);

int receveNetwork(int sock, struct sockaddr_in *adr_svr, char * message);

int sendNetwork(int sock,struct sockaddr_in *adr_svr,char * message);

void getIP(char*  myIP);

#endif /* NETWORK_H_ */
