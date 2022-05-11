/*******************************************************************************
INCLUDING HEADER FILES
*******************************************************************************/
#include "header.h"
#include "server.h"
#include "global.h"


/*******************************************************************************
GLOBAL OBJECTS
*******************************************************************************/
client_t *p_clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;


/*******************************************************************************
Function Name: str_trim_lf
Function description: it trims \n from array
Returns : void
*******************************************************************************/
void SERVER::str_trim_lf (char* arr, int length) {
	int i;
	for (i = 0; i < length; i++) { 
	if (arr[i] == '\n') {
		arr[i] = '\0';
		break;
	}
  }
}
	
	
/*******************************************************************************
Function Name: print_client_addr
Function description: prints address of socket
Returns : void
*******************************************************************************/
void SERVER::print_client_addr(struct sockaddr_in addr){
	cout <<(addr.sin_addr.s_addr & 0xff)<<"."
	<<((addr.sin_addr.s_addr & 0xff00)>>8)<<"."
	<<((addr.sin_addr.s_addr & 0xff0000)>>16)<<"."
	<<((addr.sin_addr.s_addr & 0xff000000)>>24);
}
	
	
/*******************************************************************************
Function Name: queue_add
Function description: Add p_clients to queue
Returns : void
*******************************************************************************/
void SERVER::queue_add(client_t *cl){
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i < MAX_CLIENTS; ++i){
		if(!p_clients[i]){
			p_clients[i] = cl;
			break;
		}
	}

	pthread_mutex_unlock(&clients_mutex);	
}
	
	
/*******************************************************************************	
Function Name: queue_remove
Function description: Remove p_clients from queue
Returns : void
*******************************************************************************/
void SERVER::queue_remove(int uid){
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i < MAX_CLIENTS; ++i){
		if(p_clients[i]){
			if(p_clients[i]->uid == uid){
				p_clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}
	
	
/*******************************************************************************
Function Name: send_message
Function description: Send message to all p_clients except sender
Returns : void
*******************************************************************************/
void SERVER::send_message(char *s, int uid){
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i<MAX_CLIENTS; ++i){
		if(p_clients[i]){
			if(p_clients[i]->uid != uid){
				if(write(p_clients[i]->sockfd, s, strlen(s)) < 0){
					perror("ERROR: write to descriptor failed");
					errorLog("[ERROR] write to descriptor failed");
					break;
				}
			}
		}
	}
	
	pthread_mutex_unlock(&clients_mutex);
}
	

/*******************************************************************************
Function Name: getCurrentTime
Function description: returns the current time
Returns : const char *
*******************************************************************************/
const char * SERVER::getCurrentTime(){
	time_t rawtime;
	struct tm *info;
	time(&rawtime);
	info = localtime(&rawtime);
	return asctime(info);
}
	

/*******************************************************************************
Function Name: errorLog
Function description: logs error into logfile
Returns : void
*******************************************************************************/
void SERVER::errorLog(char const *LogString){
	server_logFile = fopen("Error_Log.txt", "ab");
	fprintf(server_logFile, "%s, %s", LogString, getCurrentTime());
	fclose(server_logFile);
}


/*******************************************************************************
Function Name: infoLog
Function description: logs infomation into logfile
Returns : void
*******************************************************************************/
void SERVER::infoLog(char const *LogString){
	server_logFile = fopen("Info_Log.txt", "ab");
	fprintf(server_logFile, "%s, %s", LogString, getCurrentTime());
	fclose(server_logFile);
}
	

/*******************************************************************************
Function Name: warnLog
Function description: logs warnings into logfile
Returns : void
*******************************************************************************/
void SERVER::warnLog(char const *LogString){
	server_logFile = fopen("Warn_Log.txt", "ab");
	fprintf(server_logFile, "%s, %s", LogString, getCurrentTime());
	fclose(server_logFile);
}