#ifndef GLOBAL_H
	#define GLOBAL_H


/*******************************************************************************
GLOBAL VARIABLES
*******************************************************************************/
static unsigned int cli_count = 0; 
static int uid = 10;

extern int c_sockfd;
extern volatile sig_atomic_t flag;

static string send_username;
static string send_password;
static string db_username;
static string db_password;


/*******************************************************************************
FUNCTION PROTOTYPE
*******************************************************************************/
extern int callbackUsername(void *data, int argc, char **argv, char **azColName);
extern int callbackPassword(void *data, int argc, char **argv, char **azColName);
extern void catch_ctrl_c_and_exit(int sig);
extern void * send_msg_handler(void * arg);
extern void * recv_msg_handler(void * arg);
extern void *handle_client(void *arg);


#endif