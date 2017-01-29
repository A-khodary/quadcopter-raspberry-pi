#include "serv.h"

void clean_args_SERVER(args_SERVER * arg) {
	if (arg != NULL) {
		if (arg->pmutexRemoteConnect != NULL) {
			clean_PMutex(arg->pmutexRemoteConnect);
		}
		free(arg);
		arg=NULL;
	}
}

char isMessage(char * messageReceve, char * messageToTest) {
	char str1[SIZE_SOCKET_MESSAGE];
	char str2[SIZE_SOCKET_MESSAGE];
	int res = 0;

	strcpy(str1, messageToTest);
	strcpy(str2, messageReceve);
	res = strcmp(str1, str2);

	return res == 0;
}

char isMessagePause(char * message) {
	return isMessage(message,"PAUSE");
}

char isMessageSTOP(char * message){
	return isMessage(message,"STOP");
}

char getAdresseIP(char *message,struct sockaddr_in * sa){

	char ip[50];
	int cmp=0;
	while(*message!=' '){
		ip[cmp]=*message;
		message++;
		cmp++;
	}
	cmp++;
	ip[cmp]='\0';

	//printf("ip get : %s\n",ip);
	return inet_pton(AF_INET,(const char *) &ip, &(sa->sin_addr));
}

void MessageToStruc(char * message,int sizeFloat,DataController * dataTmp){


	//PMutex * pmutex = arg->dataController->pmutex->mutex;


	int tmp=sizeFloat;

	float a=strtof(message,0);


	float b=strtof(message+tmp-1,0);

	tmp+=sizeFloat-1;

	float c=strtof(message+tmp-1,0);

	tmp+=sizeFloat-1;

	float d=strtof(message+tmp-1,0);

	tmp+=sizeFloat-1;

	float flagTMP=strtof(message+tmp-1,0);

	char flag = (char ) flagTMP;

	dataTmp->axe_Rotation=a;
	dataTmp->axe_UpDown=b;
	dataTmp->axe_LeftRight=c;
	dataTmp->axe_FrontBack=d;
	dataTmp->flag=flag;

}


void *thread_UDP_SERVER(void *args) {

	args_SERVER *argSERV = (args_SERVER*) args;

	char verbose =argSERV->verbose;
	if(verbose){printf("THREAD SERV : SERVEUR UDP\n");}
	int sock;
	struct sockaddr_in adr_svr;

	memset(&adr_svr, 0, sizeof(adr_svr));
	adr_svr.sin_family 		= AF_INET;
	adr_svr.sin_addr.s_addr = htonl(INADDR_ANY);
	adr_svr.sin_port 		= htons(8888);

	if((sock=socket(PF_INET,SOCK_DGRAM,0)) ==-1 ){
		perror("THREAD SERV : Socket error");
	}

	if(bind(sock,(struct sockaddr *)&adr_svr,sizeof(adr_svr))){
		perror("THREAD SERV : bind error");
	}
	char buff[SIZE_SOCKET_MESSAGE];

	if(receveNetwork(sock,NULL,buff)==0){
		//TODO ERROR
	}

	buff[SIZE_SOCKET_MESSAGE-1] = '\0';
	if(verbose){printf("THREAD SERV : messag recu : %s\n",buff);}

	int fini = 1;
	int cmpNumberMessage=1;

	struct sockaddr_in  sa;
	if(getAdresseIP(buff,&sa)!=1){
		if(verbose){
			printf("ERROR IP RECEVE\n");
		}
		fini=0;//TODO
	}else{
		if (verbose) {
			printf("GOOD IP RECEVE\n");
		}
	}

	pthread_mutex_lock(&argSERV->pmutexRemoteConnect->mutex);
	pthread_cond_signal(&argSERV->pmutexRemoteConnect->condition);
	pthread_mutex_unlock(&argSERV->pmutexRemoteConnect->mutex);


	DataController dataTmp;


	while(fini){

		if (receveNetwork(sock, NULL, buff) == 0) {
			//TODO ERROR
		}

		buff[SIZE_SOCKET_MESSAGE-1] = '\0';
		if(verbose){printf("THREAD SERV : messag recu %d : %s\n",cmpNumberMessage,buff);}
		cmpNumberMessage++;

		MessageToStruc(buff, 10, &dataTmp);

		if(dataTmp.flag==1){
			if(verbose){
				printf("\nTHREAD SERV : PAUSE MESSAGE\n\n");
			}

		} else {

			if(argSERV->verbose){
				printf ("THREAD SERV : float a = %.6f  |float b = %.6f  |float c = %.6f  |float d = %.6f  | FLAG = %d\n",
						dataTmp.axe_FrontBack ,dataTmp.axe_UpDown,dataTmp.axe_LeftRight,dataTmp.axe_FrontBack,dataTmp.flag);
			}

			if (dataTmp.flag==0) {
				if (verbose) {
					printf("THREAD SERV : STOP MESSAGE\n");
				}
				fini=0;
			}

			pthread_mutex_lock(&argSERV->dataController->pmutex->mutex);


			argSERV->dataController->axe_Rotation=dataTmp.axe_Rotation;
			argSERV->dataController->axe_UpDown=dataTmp.axe_UpDown;
			argSERV->dataController->axe_LeftRight=dataTmp.axe_LeftRight;
			argSERV->dataController->axe_FrontBack=dataTmp.axe_FrontBack;
			argSERV->dataController->flag=dataTmp.flag;

			if(argSERV->dataController->pmutex->var>0){
				argSERV->dataController->pmutex->var=1;
			}

			pthread_cond_signal(&(argSERV->dataController->pmutex->condition));

			pthread_mutex_unlock(&(argSERV->dataController->pmutex->mutex));


		}

	}

	close(sock);

	if(verbose){printf("THREAD SERV : END\n");}

	return NULL;
}

void *thread_TCP_SERVER(void *args) {

	args_SERVER *argSERV = (args_SERVER*) args;
	char verbose =argSERV->verbose;
	if(verbose){printf("THREAD SERV : SERVEUR TCP\n");}

	int sock = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address_sock;
	address_sock.sin_family = AF_INET;
	address_sock.sin_port = htons(8888);

	printf("THREAD SERV : attente sur port : %d\n", 8888);

	address_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	int r = bind(sock, (struct sockaddr *) &address_sock,
			sizeof(struct sockaddr_in));
	if (r != 0) {
		return NULL;
	}
	r = listen(sock, 0);
	int connect = 1;
	while (connect) {
		struct sockaddr_in caller;
		socklen_t size = sizeof(caller);

		int sock2 = accept(sock, (struct sockaddr *) &caller, &size);
		printf("THREAD SERV : SERVEUR ACCEPT\n");

		if (sock2 < 0) {
			printf("THREAD SERV : Bind fail");

		}



		pthread_mutex_lock(&argSERV->pmutexRemoteConnect->mutex);

		pthread_cond_signal(&argSERV->pmutexRemoteConnect->condition);

		pthread_mutex_unlock(&argSERV->pmutexRemoteConnect->mutex);


		char *mess = "HELLO\n";
		int result = write(sock2, mess, strlen(mess) * sizeof(char));

		//TODO do a WHILE SUR le WRITE

		fcntl(sock2, F_SETFL, O_NONBLOCK);
		int fd_max = sock2 + 1;
		struct timeval tv;
		fd_set rdfs;

		int fini = 1;
		while (fini) {
			FD_ZERO(&rdfs);
			FD_SET(sock2, &rdfs);
			tv.tv_sec = 3;
			tv.tv_usec = 500000;
			int ret = select(fd_max, &rdfs, NULL, NULL, &tv);
			//printf("valeur de retour de select : %d\n", ret);
			if (ret == 0) {
				printf("THREAD SERV : Timed out\n");
				fini = 0;
			} else if (FD_ISSET(sock2, &rdfs)) {
				int messageRead = 0;
				int iter = 0;
				char buff[SIZE_SOCKET_MESSAGE];
				while (messageRead < 1 && iter < 10) {
					iter++;
					//printf("THREAD SERV : try to read\n");
					int bytesRead = read(sock2, buff, SIZE_SOCKET_MESSAGE - messageRead);
					messageRead += bytesRead;
				}
				if (messageRead > 0) {
					buff[SIZE_SOCKET_MESSAGE-1] = '\0';
					//printf("THREAD SERV : Message recu : %s\n", buff);
					//MessageToStruc(buff, 10, argSERV);

					char str1[2];
					char str2[2];
					int res = 0;
					strcpy(str1, "X\0");
					strcpy(str2, buff);
					res = strcmp(str1, str2);
					if (res == 0) {
						fini = 0;
						printf("THREAD SERV : c'est fini !!\n");
					}
				} else {
					printf("THREAD SERV : NOTHING TO READ !!\n");
					fini = 0;
				}
			} else {
				printf("THREAD SERV : SELECT EXIST WITHOUT GOOD VALUE\n");
			}
		}
		close(sock2);
	}
	return NULL;
}
