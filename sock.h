#ifndef SOCK_H
#define SOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int getOpenSocket(char *HOST, int PORT);
void listenFromSocket(int client_socket, char *buffer);
void sendAlertToGUI(int client_socket, int gasNumber, char alertPriority);
void getFromGUI(int client_socket);

#endif
