#ifndef MAIN_H_
#define MAIN_H_

#include <pthread.h>
#include <semaphore.h>
#include "sock.h"
#include "led_matrix.h"

char *HOST = "192.168.0.59";
// const char *HOST = "10.0.2.15";

int PORT1 = 1231;
int PORT2 = 1232;

struct ThreadSockArgs
{
    const char *host;
    int port;
};

struct ThreadLEDArgs
{
    uint8_t (*text)[CHAR_WIDTH];
    u_int16_t color;
};

pthread_t sock1_thread, sock2_thread, led_thread;

char computePriority(int);
char computeMaxPriority();
void *socketThreadFun(void *arg);
void *ledThreadFunction(void *arg);

#endif /* MAIN_H_ */
