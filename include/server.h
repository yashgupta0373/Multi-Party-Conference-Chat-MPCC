#ifndef SERVER_H
	#define SERVER_H


/*******************************************************************************
CLASS PROTOTYPE
*******************************************************************************/
class SERVER{
	
	private: 
	
	FILE *server_logFile;
	
	public:	

	void str_trim_lf (char* arr, int length);
	void print_client_addr(struct sockaddr_in addr);
	void queue_add(client_t *cl);
	void queue_remove(int uid);
	void send_message(char *s, int uid);
	const char *getCurrentTime();
	void errorLog(char const *LogString);
	void infoLog(char const *LogString);
	void warnLog(char const *LogString);
	
};


#endif