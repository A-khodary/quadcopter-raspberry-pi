/*
            +-----+-----+---------+------+---+---Pi 2---+---+------+---------+-----+-----+
            | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
            +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
            |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
            |   2 |   8 |   SDA.1 | ALT0 | 1 |  3 || 4  |   |      | 5V      |     |     |
            |   3 |   9 |   SCL.1 | ALT0 | 1 |  5 || 6  |   |      | 0v      |     |     |
            |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | ALT0 | TxD     | 15  | 14  |
        ----|     |     |      0v |      |   |  9 || 10 | 1 | ALT0 | RxD     | 16  | 15  |
        |   |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 Motor 3<---|  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |----
            |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |   |
            |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |--->Motor 1
            |  10 |  12 |    MOSI | ALT0 | 0 | 19 || 20 |   |      | 0v      |     |     |
            |   9 |  13 |    MISO | ALT0 | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
            |  11 |  14 |    SCLK | ALT0 | 0 | 23 || 24 | 1 | ALT0 | CE0     | 10  | 8   |
            |     |     |      0v |      |   | 25 || 26 | 1 | ALT0 | CE1     | 11  | 7   |
            |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
            |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
            |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
            |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |----
 Motor 4<---|  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | IN   | GPIO.27 | 27  | 16  |   |
        |   |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |--->Motor 2
        |---|     |     |      0v |      |   | 39 || 40 | 0 | IN   | GPIO.29 | 29  | 21  |
            +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
            | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
            +-----+-----+---------+------+---+---Pi 2---+---+------+---------+-----+-----+
 */


#ifndef _TYPES_H_
#define _TYPES_H_
#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/mman.h>

struct motor_info{
    int broche;
    double high_time;
    double low_time;
    pthread_mutex_t * lock;
};
//Change la puissance d'un moteur, power en % (de 0% a 10%),renvoi 1 si echec.
int  set_power(struct  motor_info * info,double power);
void init_motor_info(struct motor_info *info,int broche, pthread_mutex_t * lock1);
//Initialise les 4 moteur a 0% de puissance(4 thread en RT et sur le coeur 1).
void init_motors(struct motor_info * info_m0,struct motor_info * info_m1,struct motor_info * info_m2,struct motor_info * info_m3,
                 pthread_mutex_t * lock_m0,pthread_mutex_t *lock_m1,pthread_mutex_t *lock_m2,pthread_mutex_t *lock_m3);

    
#endif