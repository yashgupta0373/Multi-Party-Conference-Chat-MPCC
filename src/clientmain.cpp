/*******************************************************************************
INCLUDING HEADER FILES
*******************************************************************************/
#include "header.h"
#include "client.h"
#include "database.h"
#include "global.h"


/*******************************************************************************
GLOBAL VARIABLE
*******************************************************************************/
int c_sockfd = 0;
volatile sig_atomic_t flag = 0;


/*******************************************************************************
Function Name: main
Function description: main function
Returns : int
*******************************************************************************/
int main(int argc, char **argv)				
{
	
	DATABASE dbObj;
	
	if(argc != 2)						
	{
		cout<<"Usage:" << argv[0]<< "<port>" <<endl;
		return EXIT_FAILURE;
	}
	
	int choice;
	int f;	
	const char *ip = "127.0.0.1";
	int port = atoi(argv[1]);

	signal(SIGINT, catch_ctrl_c_and_exit);
	
	struct sockaddr_in server_addr;

	/* Socket settings */
	c_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  	server_addr.sin_family = AF_INET;
  	server_addr.sin_addr.s_addr = inet_addr(ip);
  	server_addr.sin_port = htons(port);
  	
  	/*MENU for the client*/
	do{
	
	cout<<"\n*********************** CONFERENCE CHATROOM DASHBOARD ***********************"<<endl;
	cout<<"\nPlease Enter Your choice : "<< endl;
	cout<<"1.New User\n2.Login\n3.Exit" <<endl;
	cin>>choice;
	
	if(choice==1)
		dbObj.userRegisterationtoDB();	
			
	else if(choice==2)
	{
		f = dbObj.userAuthenticationfromDB();
		break;
	}		
			
	else if(choice==3)
		exit(0);	
			
	else
		cout<<"Invalid choice! Please try Again" <<endl;
	
	}while(choice==1);	
	
	if(f == 1)
	{
		
	    // Connect to Server
	    int err = connect(c_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	    if (err == -1) {
			cout<<"ERROR: connect"<<endl;
			return EXIT_FAILURE;
		}
		
		dbObj.sendUsername(send_username);
		
		cout<<"\n********************** WELCOME TO CONFERENCE CHATROOM **********************\n"<<endl;

		pthread_t send_msg_thread;
		
	    if(pthread_create(&send_msg_thread, NULL,&send_msg_handler, NULL) != 0)
	    {
			cout<<"ERROR: pthread"<<endl;
			return EXIT_FAILURE;
	    }

		pthread_t recv_msg_thread;
		
	    if(pthread_create(&recv_msg_thread, NULL, &recv_msg_handler, NULL) != 0){
			cout<<"ERROR: pthread"<<endl;
			return EXIT_FAILURE;
		}

		while (1){
			if(flag){
				cout<<"\nBye"<<endl;
				break;
			}
		}

		close(c_sockfd);
		return EXIT_SUCCESS;
	}
}