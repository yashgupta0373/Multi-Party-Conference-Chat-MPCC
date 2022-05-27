#ifndef CLIENT_H
	#define CLIENT_H


/*******************************************************************************
CLASS PROTOTYPE
*******************************************************************************/
class CLIENT{
	
	private:
	
	FILE *client_logFile;
	
	public:
	
	void print_stdout();
	void str_trim_lf (char * arr, int length);
	const char *getCurrentTime();
	void errorLog(char const *LogString);
	void infoLog(char const *LogString);
	void warnLog(char const *LogString);

};


#endif