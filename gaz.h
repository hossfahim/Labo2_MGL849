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


#define POLICY SCHED_RR
//#define POLICY SCHED_FIFO
#define THREADSTACK 65536


/*typedef struct gaz_struct {
	float		data[4];
	float 		evol;
	int action;
	pthread_t	Thread;
	sem_t   SemThread;
} GazStruct;*/


int GazInit ();
int GazStart (void);
int GazStop (int num);

#endif /* GAZ_H_ */
