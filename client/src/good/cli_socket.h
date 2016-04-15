#ifndef CLI
#define CLI
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include "header_cli.h"

int writen(int sock, char *buf, int len);
int readn(int sock, char *buf, int len);
int sendData(int sock, void * data, sect_type sect, int size);
int retrieveData(int sock, sect_type sector, void ** ris);
int serverConnectionInit(char *ip_addr,char *port, int *socket_main);
int serverConnectionFinish(int socketfd);

#endif
