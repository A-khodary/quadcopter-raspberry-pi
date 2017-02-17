#include "client.h"

int main (int argc, char *argv[]){
	if(argc<2){
		perror("Indiquer l'adresse IP du drone en argument");
		return EXIT_FAILURE;
	}
	char * adresse=argv[1];
	char verbose=0;
	setVerbose(&verbose,argc,argv[2],2);


	char array[400];
	sprintf(array, "THREAD MAIN : adresse choisit : %s\n",argv[1]);
	logString(array);

	args_CONTROLER * argControler;
	if(init_args_CONTROLER(&argControler)){
		return EXIT_FAILURE;
	}

	args_CLIENT * argClient;
	if(init_args_CLIENT(&argClient,adresse,argControler)){
		return EXIT_FAILURE;
	}

	pthread_t threadClient;
	pthread_t threadControler;

	logString("THREAD MAIN : TEST MANETTE\n");

	pthread_mutex_lock(&argControler->pmutexControllerPlug->mutex);

	if (pthread_create(&threadControler, NULL, thread_XBOX_CONTROLER,argControler)) {
		logString("THREAD MAIN : pthread_create");
		return EXIT_FAILURE;
	}

	//wait for XBOX CONTROLER
	pthread_cond_wait(&argControler->pmutexControllerPlug->condition, &argControler->pmutexControllerPlug->mutex);

	pthread_mutex_unlock(&argControler->pmutexControllerPlug->mutex);

	if(verbose){printf("MANETTE ok \n");}

	//XBOX CONTROLER IS ON , we can start the client socket thread
	if (pthread_create(&threadClient, NULL, thread_UDP_CLIENT, argClient)) {
		logString("pthread_create");
		return EXIT_FAILURE;
	}

	if (pthread_join(threadClient, NULL)) {
		logString("pthread_join SERV");
		return EXIT_FAILURE;
	}

	argControler->endController=1;

	if (pthread_join(threadControler, NULL)) {
		logString("pthread_join CONTROLER");
		return EXIT_FAILURE;
	}


	clean_args_CLIENT(argClient);
	clean_args_CONTROLER(argControler);
	logString("THREAD MAIN : END\n");

	return EXIT_SUCCESS;
}
