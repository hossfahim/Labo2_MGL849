/*
 * gaz.h
 *
 *  Created on: 29/02/24
 *      Author: Baptiste
 */

#ifndef GAZ_H_
#define GAZ_H_

#include <stdio.h>   /* Standard input/output definitions */
#include <stdlib.h>
#include <stdint.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <signal.h>
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <math.h>



#endif /* GAZ_H_ */

typedef struct gaz_struct {
	int		data[4];
	pthread_mutex_t mutexData;
	pthread_mutex_t mutexEvol;
	float 		evol;
	int action;
	pthread_t	Thread;
	sem_t   semThread;
}GazStruct;



extern pthread_barrier_t GazDecisionBarrier;
extern pthread_barrier_t GazStartBarrier;
extern GazStruct gaz[3];

