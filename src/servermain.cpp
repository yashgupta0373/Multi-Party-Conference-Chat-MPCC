/*******************************************************************************
INCLUDING HEADER FILES
*******************************************************************************/
#include "header.h"
#include "server.h"
#include "global.h"


SERVER serverObj;


/*******************************************************************************
Function Name: handle_client
Function description: Handle all communication with the client
Returns : void *
*******************************************************************************/
void *handle_client(void *arg){
	char buff_out[S_BUFFER];
	char name[32];
	int leave_flag = 0;

	cli_count++;
	client_t *cli = (client_t *)arg;

	if(recv(cli->sockfd, name, 32, 0) <= 0 || strlen(name) <  2 
	|| strlen(name) >= 32-1){
		
		cout<<"Didn't enter the name.\n";
		serverObj.warnLog("[WARN] Didn't enter the name");
		leave_flag = 1;
		
	} else{
		
		strcpy(cli->name, name);
				
		sprintf(buff_out,"%shas joined\n", cli->name); 
		serverObj.infoLog("[INFO] New Client Joined");               				
		cout<<buff_out;
		serverObj.send_message(buff_out, cli->uid);	
		
	}

	bzero(buff_out, S_BUFFER);

	while(1){
		if (leave_flag) {
			break;
		}

		int receive = recv(cli->sockfd, buff_out, S_BUFFER, 0);
		if (receive > 0){
			if(strlen(buff_out) > 0){
				serverObj.send_message(buff_out, cli->uid);
				serverObj.str_trim_lf(buff_out, strlen(buff_out));
				cout<<buff_out<<endl;
			}
		} else if (receive == 0 || strcmp(buff_out, "exit") == 0){
			sprintf(buff_out, "%shas left\n", cli->name);
			serverObj.infoLog("[INFO] A Client Left");
			cout<<buff_out;
			serverObj.send_message(buff_out, cli->uid);
			leave_flag = 1;
		} else {
			cout<<"ERROR: -1\n";
			serverObj.errorLog("[ERROR] -1");
			leave_flag = 1;
		}

		bzero(buff_out, S_BUFFER);
	}

	close(cli->sockfd);	
	serverObj.queue_remove(cli->uid);
	delete cli;
	cli_count--;
	pthread_detach(pthread_self());

	return NULL;
}


/*******************************************************************************
Function Name: main
Function description: main function
Returns : int
*******************************************************************************/
int main(int argc, char **argv){
	
	if(argc != 2){
		cout<<"Port Number Missing (server <port>)";
		serverObj.warnLog("[WARN] Port Number Missing (server <port>)");
		return EXIT_FAILURE;
	}
	
	const char *ip = "127.0.0.1";		
	int port = atoi(argv[1]);		
	int option = 1;
	int listenfd = 0, connfd = 0;		
	struct sockaddr_in serv_addr;       
 	struct sockaddr_in cli_addr;		
 	pthread_t tid;				

 	/* Socket settings */
  	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
  	serv_addr.sin_addr.s_addr = inet_addr(ip);
  	serv_addr.sin_port = htons(port);

  	/* Ignore pipe signals */
	signal(SIGPIPE, SIG_IGN);

	if(setsockopt(listenfd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),
		(char*)&option,sizeof(option)) < 0){
			
		perror("ERROR: setsockopt failed");
		serverObj.errorLog("[ERROR] setsockopt failed");
		return EXIT_FAILURE;
		
	}

	/* Bind */
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {   
		perror("ERROR: Socket binding failed");
		serverObj.errorLog("[ERROR] Socket binding failed");
		return EXIT_FAILURE;
	}

	/* Listen */
	if (listen(listenfd, 10) < 0) {               
		perror("ERROR: Socket listening failed");
		serverObj.errorLog("[ERROR] Socket listening failed");
		return EXIT_FAILURE;
	}

	cout<<"********************** WELCOME TO CHATROOM BROADCAST **********************"<<endl;
	serverObj.infoLog("[INFO] Chatroom Opened");

	while(1){															
		socklen_t clilen = sizeof(cli_addr);								
		connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

		if((cli_count + 1) == MAX_CLIENTS){
			cout<<"Max clients reached. Rejected: ";
			serverObj.errorLog("[ERROR] Max clients reached");
			serverObj.print_client_addr(cli_addr);
			cout<<":"<<cli_addr.sin_port<<endl;
			close(connfd);
			continue;
		}

		client_t *cli = new client_t;
		cli->address = cli_addr;
		cli->sockfd = connfd;
		cli->uid = uid++;

		serverObj.queue_add(cli);
		serverObj.infoLog("[INFO] New client added");
		pthread_create(&tid, NULL, &handle_client, (void*)cli); 

		sleep(1);
	}

	return EXIT_SUCCESS;
}