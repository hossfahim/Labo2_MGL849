#include "main.h"
#include <stdlib.h>
#include <time.h>

#include "gaz.h"
#include "extern.h"
#include "finalDecision.h"


uint8_t L_LED[CHAR_HEIGHT][CHAR_WIDTH] = {
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}};

uint8_t M_LED[CHAR_HEIGHT][CHAR_WIDTH] = {
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1}};

uint8_t H_LED[CHAR_HEIGHT][CHAR_WIDTH] = {
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1}};

char buffer[BUFFER_SIZE];
int blinkFreq = 250;

int terminated = 1;

// typedef struct gaz_struct {
// float		data[4];
// sem_t semData;
// float 		evol;
// int action;
// pthread_t	Thread;
// sem_t   SemThread;
//} GazStruct;

// GazStruct gaz[3];

char priorityLetterToBlink;

int main()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gaz[i].data[j] = -1;
        }
        printf("INIT%d : {%d, %d, %d, %d}\n", i, gaz[i].data[0], gaz[i].data[1], gaz[i].data[2], gaz[i].data[3]);
    }
    printf("Lancement\n");
    int client_socket = getOpenSocket(HOST, PORT1);
    int client_socket2 = getOpenSocket(HOST, PORT2);
    printf("Socket\n");

    // TEST

    GazInit();
    GazStart();
    FinalDecisionInit(client_socket2);
    FinalDecisionStart();

    // LED
    if (pthread_create(&led_thread, NULL, ledThreadFunction, NULL) != 0)
    {
        perror("pthread_create");
        return 1;
    }
    terminated = 0;

    // socket 2
    struct ThreadSockArgs args2 = {HOST, PORT2};
    if (pthread_create(&sock2_thread, NULL, socketThreadFun, (void *)&args2) != 0)
    {
        perror("pthread_create");
        return 1;
    }

    while (1)
    {
        getFromGUI(client_socket);
    }

    for (int i = 0; i < 3; i++)
        GazStop(i);

    //// Wait for all threads to finish
    // pthread_join(sock1_thread, NULL);
    pthread_join(sock2_thread, NULL);
    pthread_join(led_thread, NULL);
    terminated = 1;

    return 0;
}

void *socketThreadFun(void *arg)
{
    // To Cast the argument back to the structure type
    // Cast the void pointer to sched_param pointer
    struct sched_param *param = (struct sched_param *)arg;
    // Set the scheduling parameters for this thread
    pthread_setschedparam(pthread_self(), SCHED_FIFO, param);

    struct ThreadSockArgs *args = (struct ThreadSockArgs *)arg;

    const char *host = args->host;
    int port = args->port;

    printf("host: %s, port: %d\n", host, port);

    int client_socket = getOpenSocket(host, port);

    // if (port == PORT1)
    // {
    //     listenFromSocket(client_socket, buffer);
    // }

    if (port == PORT2)
    {

        while (1)
        {
            priorityLetterToBlink = computeMaxPriority();
            char priorityLetter;

            for (int i = 0; i < 3; i++)
            {
                if (gaz[i].data && gaz[i].data[0] >= 0)
                {
                    priorityLetter = computePriority(gaz[i].data[0]);
                    sendAlertToGUI(client_socket, i + 1, priorityLetter);
                }
            }

            delay(200);
        }
    };

    pthread_exit(NULL);
}

void *ledThreadFunction(void *arg)
{

    initLED();

    while (!terminated)
    {
        while (priorityLetterToBlink == NULL)
        {
            memset(map, 0, FILESIZE);
        }
        while (priorityLetterToBlink == 'L')
        {
            displayLetter(L_LED, BLUE);
            delay(blinkFreq);
        }
        while (priorityLetterToBlink == 'M')
        {
            displayLetter(M_LED, GREEN);
            delay(blinkFreq);
        }
        while (priorityLetterToBlink == 'H')
        {
            displayLetter(H_LED, RED);
            delay(blinkFreq);
        }
    }

    unMapLED();

    pthread_exit(NULL);
}

char computePriority(int gasValue)
{
    char priorityLetter;

    if (gasValue >= 0 && gasValue <= 5)
        priorityLetter = NULL;
    else if (gasValue >= 6 && gasValue <= 20)
        priorityLetter = 'L';
    else if (gasValue >= 21 && gasValue <= 50)
        priorityLetter = 'M';
    else if (gasValue >= 51 && gasValue <= 100)
        priorityLetter = 'H';

    return priorityLetter;
}

char computeMaxPriority()
{
    int max = gaz[0].data[0];

    if (gaz[1].data[0] > max)
    {
        max = gaz[1].data[0];
    }

    if (gaz[2].data[0] > max)
    {
        max = gaz[2].data[0];
    }

    return computePriority(max);
}
