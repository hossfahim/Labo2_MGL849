#ifndef FINALDECISION_H_
#define FINALDECISION_H_

#define POLICY2 SCHED_RR
//#define POLICY SCHED_FIFO
#define THREADSTACK2 65536



void *finalDecisionTask(void *ptr);
int FinalDecisionInit (int socket2);
int FinalDecisionStart (void);

#endif
