#ifndef HEADER_H
	#define HEADER_H


/*******************************************************************************
INCLUDING HEADER FILES
*******************************************************************************/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <bits/stdc++.h>
#include <sqlite3.h>

using namespace std;


/*******************************************************************************
INCLUDING MACROS
*******************************************************************************/
#define MAX_CLIENTS 100
#define S_BUFFER 2048

#define LENGTH 2048
#define C_BUFFER 8192


/*******************************************************************************
STRUCTURE DECLERATION
*******************************************************************************/
typedef struct{
	struct sockaddr_in address;  
	int sockfd;
	int uid;
	char name[32];  
} client_t;


#endif