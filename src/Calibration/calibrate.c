#include "calibrate.h"


void emptyBuffer(void){
    int c;
    while((c=getchar()) != EOF && c != '\n');
}

int input_test(char value_Wait) {
	char tmp=255;
	int first=1;
	do {
		if(tmp=='e'){
			return -1;
		}

		if(!first){
			printf("WRONG KEY !! push : %c  ",value_Wait);
		}
		first=0;
		scanf("%c", &tmp);
		emptyBuffer();
	} while (tmp != value_Wait);

	return 0;
}


void test_Power(MotorsAll * motorsAll3){

	int power[NUMBER_OF_MOTORS];
	int valueAsk=MOTOR_LOW_TIME;

	int palier=50;
	sleep(3);
	fflush(NULL);
	printf("******************\nPOWER TEST\n******************\n");
	do{
		for (int i = 0; i < NUMBER_OF_MOTORS; i++) {
			power[i] = valueAsk;
		}
		fflush(NULL);
		printf("\nEnter u , if you want upgrade of %d OR e for exit\n",palier);
		if(input_test('u')){
			return;
		}
		valueAsk+=palier;
		printf("ACTUAL VALUE : %d",valueAsk);
		set_power(motorsAll3,power);

	}while(valueAsk<=MOTOR_HIGH_TIME);

}

void calibrate_ESC(MotorsAll * motorsAll3,char verbose){

	/*
	int powerMax[4] = { MOTOR_HIGH_TIME, MOTOR_HIGH_TIME, MOTOR_HIGH_TIME, MOTOR_HIGH_TIME };
	int powerMin[4] = { MOTOR_LOW_TIME, MOTOR_LOW_TIME, MOTOR_LOW_TIME, MOTOR_LOW_TIME };
	*/

	int powerMax[NUMBER_OF_MOTORS];
	int powerMin[NUMBER_OF_MOTORS];
	for(int i=0;i<NUMBER_OF_MOTORS;i++){
		powerMax[i]=MOTOR_HIGH_TIME;
		powerMin[i]=MOTOR_LOW_TIME;
	}


	if (verbose) {
		sleep(3);
		fflush(NULL);
		printf("******************\nESC CALIBRATION\n******************\n");
		printf("THE BATTERY OF ESC NEED TO BE DISCONNECT FIRST ,when it's done : press the key y and ENTER\n");
		input_test('y');
	}
	set_power(motorsAll3,powerMax);
    if(verbose){
    	printf("\nNOW CONNECT THE BATTERY, when it's done : press the key y and ENTER\n");
    	input_test('y');
    	printf("\nNOW WAIT FOR THE 3 BIP, when it's over : press the key y and ENTER\n");
    }else{
    	system(SOUND_COMMAND"connectTheBattery.mp3");
    	sleep(10);
    	system(SOUND_COMMAND"calibrationStart.mp3");
    }
    set_power(motorsAll3,powerMin);
	if (verbose) {
		input_test('y');
		printf("\nCALIBRATION FINISH\n");
	}else{
		sleep(10);
		system(SOUND_COMMAND"calibrationFinish.mp3");
	}

}
