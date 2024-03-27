/*
 * Gaz.c
 *
 *  Created on: 29/02/24
 *      Author: Baptiste
 */


#include "gaz.h"
#include "extern.h"

int GazActivated = 0;
int count = 0;



GazStruct gaz[3];


void *GazTask ( void * ptr ) {
	
	int num = -1;
	num = *(int*)ptr;
	//memcpy(&num, ptr, sizeof(int));

	printf("%s Gaz%d Init\n", __FUNCTION__, num);
	
	
	//printf("%s %d\n", __FUNCTION__, *num);
	
	pthread_barrier_wait(&GazStartBarrier);
	
	while(GazActivated == 1){
		
		printf("%s Gaz%d Wait\n", __FUNCTION__, num);
		
		sem_wait(&(gaz[num].semThread));
		printf("%s Gaz%d Free\n", __FUNCTION__, num);
		if (GazActivated == 0)
					break;
		
		printf("%s, %d \n", __FUNCTION__, count++);
		
		
		// récuperer nouvelle data
		int data[4];
		//sem
		pthread_mutex_lock(&gaz[num].mutexData);
		printf("Data%d : {%d, %d, %d, %d}\n", num, gaz[num].data[0], gaz[num].data[1], gaz[num].data[2], gaz[num].data[3]); 
		//memcpy(data, gaz[num].data, sizeof(gaz[num].data)); //PROTECTION NEEDED
		for(int l=0;l<4;l++)
			data[l] = gaz[num].data[l];
		pthread_mutex_unlock(&gaz[num].mutexData);
		//sem
		for(int j=0;j<4;j++){
				printf("%s%d, data : %d \n", __FUNCTION__,num, data[j]); 
		}
		
		
		//calcul evolution sans ponderation
		/*float evol[3];
		
		for(int i=0;i<4;i++){
			evol[i] = (data[i+1]-data[i])/data[i];
		}
		* */
		float evolGlobal = 0;
		/*
		for(int i=0;i<4;i++){
			evolGlobal += evol[i];
		}
		evolGlobal = evolGlobal / 4; //evolution non pondéré*/
		//evolGlobal = (data[3]-data[0])*data[0]; // 1 - 384
		
		
		
		evolGlobal = (data[0]-data[3])/4.0;
		
		
		printf("%s%d, evolGlobal : %f \n", __FUNCTION__,num, evolGlobal);
		
		//Prise décision individuelle
		float diff  = evolGlobal - gaz[num].evol;
		if(diff > 0 && evolGlobal>0){
			
			
			if(diff > 8){
			
				gaz[num].action +=3;

			}else if(diff > 3){
				gaz[num].action +=2;
			
			}else{
				gaz[num].action ++;
			}
			
			if(gaz[num].action>5)
					gaz[num].action = 5;
		}else {
			
			//Nothing
			if(gaz[num].data[3] <=0)
				gaz[num].action = 0;
			
		}
		pthread_mutex_lock(&gaz[num].mutexEvol);
		gaz[num].evol = evolGlobal;
		pthread_mutex_unlock(&gaz[num].mutexData);
		printf("%s%d, Action required : %d \n", __FUNCTION__,num, gaz[num].action);
		
		//Rejoindre la barrier
		pthread_barrier_wait(&GazDecisionBarrier);
		pthread_barrier_wait(&GazStartBarrier);
		
	}
	
	printf("%s : Terminé\n", __FUNCTION__);
	pthread_exit(0); /* exit thread */
	
}


int GazInit () {
	
	//int num = 0; //ENLEVER

	pthread_attr_t		attr;
	struct sched_param	param;
	int					minprio, maxprio;
	
	int cr = pthread_barrier_init(&GazStartBarrier, NULL, 4);
	
	
	pthread_attr_init(&attr);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	minprio = sched_get_priority_min(POLICY);
	maxprio = sched_get_priority_max(POLICY);
	pthread_attr_setschedpolicy(&attr, POLICY);
	param.sched_priority = maxprio - 15; //minprio + (maxprio - minprio)/2;
	pthread_attr_setstacksize(&attr, THREADSTACK);
	pthread_attr_setschedparam(&attr, &param);

	for(int i=0;i<3;i++){
		sem_init(&(gaz[i].semThread), 0, 0);
		//pthread_mutex_init(&(gaz[i].semThread),NULL);
		//sem_init(&(gaz[i].semData), 0, 0);
		pthread_mutex_init(&(gaz[i].mutexData),NULL);
		gaz[i].action = 0;
		gaz[i].evol = 0;

		pthread_create( &gaz[i].Thread, &attr, &GazTask, &i);
	
	}
	pthread_attr_destroy(&attr);
	
	


	return 0;
}



int GazStart (void) {
                                            
	GazActivated = 1;
	printf("%s GazActivated\n", __FUNCTION__);
	pthread_barrier_wait(&GazStartBarrier);

	pthread_barrier_destroy(&GazStartBarrier);
	printf("%s Gaz démarré\n", __FUNCTION__);

	return 0;
}



int GazStop (int num) {


	GazActivated = 0;
	//int num = 0; //ENLEVER

	sem_post(&(gaz[num].semThread));

	pthread_join(gaz[num].Thread,NULL);
	
	sem_destroy(&(gaz[num].semThread));
	pthread_mutex_destroy(&(gaz[num].mutexData));
	printf("%s Gaz arreté \n", __FUNCTION__);
	
	return 0;
}

