
#include "sock.h"
#include "extern.h"

//extern GazStruct gaz[3];
   //for(int i=0;i<2;i++){
//for(int j=0;j<3;j++){
//gaz[i].data[j] = -1;
//}}

int getOpenSocket(char *HOST, int PORT)
{

    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(HOST);
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error connecting to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    return client_socket;
}

void listenFromSocket(int client_socket, char *buffer)
{
    printf("Start Receiving response from - %d\n", client_socket);

    int bytes_received;
    while (1)
    {
        // Receive data from socket
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0)
        {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }
        else if (bytes_received == 0)
        {
            printf("Server closed connection\n");
            break; // Exit loop if server closed connection
        }
        else
        {
            buffer[bytes_received] = '\0'; // Null-terminate the received data
            // printf("Received: %s\n", buffer);
        }
    }
}

void sendAlertToGUI(int client_socket, int gasNumber, char alertPriority)
{
    char Buff[10];

    if (alertPriority)
        sprintf(Buff, "AG%d%c\n", gasNumber, alertPriority);
    else
        sprintf(Buff, "AG%d\n", gasNumber);

    if (send(client_socket, Buff, strlen(Buff), 0) == -1)
    {
        perror("Error sending data");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
}

void getFromGUI(int client_socket)
{
    
    int bytes_received;
    char buffer[BUFFER_SIZE];
    char* sep ;
    

   
// GET GAS  Values from GUI
 // Receive data from socket
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0)
        {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }
        else if (bytes_received == 0)
        {
            printf("Server closed connection\n");
            exit(-1); // Exit loop if server closed connection
        }
        else
        {
            buffer[bytes_received] = '\0'; // Null-terminate the received data
  char temp[3];      
  char * pch;
  //printf ("W%s\n",buffer);
  pch = strtok (buffer,"\n");
  while (pch != NULL)
  {
    //printf ("%s",buffer);
int value;
int num = (int)pch[2] - 48 - 1;
//printf("*****%s\n*****",pch);
pch[2]='X';
//printf("--------%s-----\n",pch);
sscanf(pch,"LGX%d", &value);

  printf("LG%d:%d\n", num, value); 
  
  pthread_mutex_lock(&gaz[num].mutexData);
  for(int i = 2; i>=0; i--){
      gaz[num].data[i+1] = gaz[num].data[i];
    }
    gaz[num].data[0] = value; 
  printf("LG%d : {%d, %d, %d, %d}\n", num, gaz[num].data[0], gaz[num].data[1], gaz[num].data[2], gaz[num].data[3]); 
    
    if(gaz[num].data[3]!=-1)
        sem_post (&(gaz[num].semThread));
    pthread_mutex_unlock(&gaz[num].mutexData);
    
    pch = strtok (NULL, "\n");
  }
           
}
        


}


