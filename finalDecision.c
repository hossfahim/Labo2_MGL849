#include "finalDecision.h"

#include "extern.h"
#include "gaz.h"
#include "sock.h"

#include <pthread.h>

int finalDecisionActivated = 0;
pthread_t	finalDecision_Thread;



void *finalDecisionTask(void *ptr)
{

  int client_socket = 0;
  client_socket = *(int *)ptr;
  int actionTable[3];
  int decision = 0;
  char Buff[10];

  pthread_barrier_wait(&GazDecisionBarrier);
  
while(finalDecisionActivated == 1){
  for (int i = 0; i < 2; i++)
  {
    actionTable[i] = gaz[i].action;
  }

  for (int i = 1; i < 2; i++)
  {
    if (decision < actionTable[i])
    {
      decision = actionTable[i];
    }
  }
  switch (decision)
  {
  case 1:

    sprintf(Buff, "AL1\n");

    if (send(client_socket, Buff, strlen(Buff), 0) == -1)
    {
      perror("Error sending data");
      close(client_socket);
      exit(EXIT_FAILURE);
    }
    break;
  case 2:

    sprintf(Buff, "AL2\n");

    if (send(client_socket, Buff, strlen(Buff), 0) == -1)
    {
      perror("Error sending data");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    break;

  case 3:

    sprintf(Buff, "AL3\n");

    if (send(client_socket, Buff, strlen(Buff), 0) == -1)
    {
      perror("Error sending data");
      close(client_socket);
      exit(EXIT_FAILURE);
    }
    break;

  case 4:

    sprintf(Buff, "VL1\n");

    if (send(client_socket, Buff, strlen(Buff), 0) == -1)
    {
      perror("Error sending data");
      close(client_socket);
      exit(EXIT_FAILURE);
    }
    break;

  case 5:

    sprintf(Buff, "VL2\n");

    if (send(client_socket, Buff, strlen(Buff), 0) == -1)
    {
      perror("Error sending data");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    break;
  }
  // Code to remove action

  pthread_barrier_wait(&GazStartBarrier);
  pthread_barrier_wait(&GazDecisionBarrier);
}
}

int FinalDecisionInit (int socket2) {
	
	//int num = 0; //ENLEVER

	pthread_attr_t		attr;
	struct sched_param	param;
	int					minprio, maxprio;	
	int cr = pthread_barrier_init(&GazDecisionBarrier, NULL, 4);
	
	pthread_attr_init(&attr);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	minprio = sched_get_priority_min(POLICY2);
	maxprio = sched_get_priority_max(POLICY2);
	pthread_attr_setschedpolicy(&attr, POLICY2);
	param.sched_priority = maxprio - 15; //minprio + (maxprio - minprio)/2;
	pthread_attr_setstacksize(&attr, THREADSTACK2);
	pthread_attr_setschedparam(&attr, &param);


		if (pthread_create( &finalDecision_Thread, &attr, &finalDecisionTask, &socket2)!=0);
     {
        perror("pthread_create final decision");
        return 1;
    }
	  pthread_attr_destroy(&attr);
	
	


	return 0;
}

int FinalDecisionStart (void) {
                                            
	finalDecisionActivated = 1;
	printf("%s FinalDecisionActivated\n", __FUNCTION__);
	pthread_barrier_wait(&GazDecisionBarrier);

	pthread_barrier_destroy(&GazDecisionBarrier);
	printf("%s Final démarré\n", __FUNCTION__);

	return 0;
}
