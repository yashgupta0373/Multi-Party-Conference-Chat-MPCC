/*******************************************************************************
INCLUDING HEADER FILES
*******************************************************************************/
#include "header.h"
#include "client.h"
#include "database.h"
#include "global.h"


/*******************************************************************************
GLOBAL OBJECTS
*******************************************************************************/
CLIENT clientObj;


/*******************************************************************************
Function Name: print_stdout
Function description: prints > after every new message line
Returns : void
*******************************************************************************/
void CLIENT::print_stdout()
{
	printf("%s","> ");
	fflush(stdout);
}


/*******************************************************************************
Function Name: str_trim_lf
Function description: it trims \n from array
Returns : void
*******************************************************************************/
void CLIENT::str_trim_lf(char * arr, int length) {
	int i;
	for (i = 0; i < length; i++) 
	{ 
		if (arr[i] == '\n') 
		{
			arr[i] = '\0';
			break;
		}
	}
}


/*******************************************************************************
Function Name: catch_ctrl_c_and_exit
Function description: Exits the chat when Ctrl+C is pressed
Returns : void
*******************************************************************************/
void catch_ctrl_c_and_exit(int sig) 
{
	flag = 1;
} 
 
 
/*******************************************************************************
Function Name: send_msg_handler
Function description: sends message to the socket
Returns : void *
*******************************************************************************/
void * send_msg_handler(void * arg) 
{
	char message[LENGTH] = {};
	char buffer[C_BUFFER] = {};

	while(1) {
		clientObj.print_stdout();
		
		fgets(message, LENGTH, stdin);
		
		clientObj.str_trim_lf(message, LENGTH);

		if (strcmp(message, "exit") == 0) 
		{
			break;
		} 
		else 
		{
	  		sprintf(buffer, "%s: %s", send_username.c_str(), message);
	  		send(c_sockfd, buffer, strlen(buffer), 0);
		}

		bzero(message, LENGTH);
		bzero(buffer, C_BUFFER);
		
	}
	catch_ctrl_c_and_exit(2);
	return NULL;
}


/*******************************************************************************
Function Name: recv_msg_handler
Function description: recieves message from the socket
Returns : void *
*******************************************************************************/
void * recv_msg_handler(void * arg) 
{
	char message[LENGTH] = {};
	while (1) {
		int receive = recv(c_sockfd, message, LENGTH, 0);
		if (receive > 0) 
		{
			cout<<message<<endl;
			clientObj.print_stdout();
		} 
		else if (receive == 0) 
		{
			break;
		} 
		else {
			cout<<"ERROR: Socket didn't recieve message"<<endl;
			clientObj.errorLog("[ERROR] Socket didn't recieve message");
		}
		memset(message, 0, sizeof(message));
	}
	return NULL;
}


/*******************************************************************************
Function Name: userRegisterationtoDB
Function description: registers new users into the database
Returns : void
*******************************************************************************/
void DATABASE::userRegisterationtoDB()
{
	string registerUsername;
	string registerPassword;
	string checkPassword;
	
    sqlite3 *DB;  
    char *messaggeError;
    int exit = 0;
    	      
    cout << "*************************** USER REGISTRATION PAGE ***************************"<<endl;
    
    cout << "\nEnter Username :";
    cin >> registerUsername;
    
    int n=registerUsername.length();
    char char_arr[n+1];
    strcpy(char_arr,registerUsername.c_str());
    clientObj.str_trim_lf(char_arr, LENGTH);
    
    registerPassword = getpass("Enter new password :");
    checkPassword = getpass("Confirm password :");
    
    int val=registerPassword.compare(checkPassword);

	if(val==0)
	{
	    exit = sqlite3_open("example.db", &DB);

	    if (exit)
	    {
		cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;	
		clientObj.errorLog("[ERROR] unable to open database");
	    }
	    else{
			clientObj.infoLog("[INFO] Opened Database Successfully!");
		}
	    
	    
	    string sql1 = "CREATE TABLE IF NOT EXISTS registeredUsers ("
		          "USERNAME        STRING	UNIQUE, "     
		          "PASSWORD         STRING );";
					  
	    int exit2 = 0;
	    exit2 = sqlite3_exec(DB, sql1.c_str(), NULL, 0, &messaggeError);

	    if (exit2 != SQLITE_OK)
	    {
		cerr << "Error Create Table" << endl;
		sqlite3_free(messaggeError);
	    }
	    else{
			clientObj.infoLog("[INFO] Table updated Successfully!");
		}
	   
	    string sql2("INSERT INTO registeredUsers (USERNAME,PASSWORD) VALUES ('" 
					+ registerUsername + "','" + registerPassword + "');");

	    int exit3 = 0;
	    exit3 = sqlite3_exec(DB, sql2.c_str(), NULL, 0, &messaggeError);
	    if (exit3 != SQLITE_OK)
	    {
		cerr << "Error Insert : Username is already taken. Please Try Again!"<<endl;
		
		sqlite3_free(messaggeError);
	    }
	    else{
		clientObj.infoLog("[INFO] Records created Successfully!");
		cout << "You are successfully enrolled to the system" << endl;
		cout << "You'll be redirected to the Menu"<< endl;
		cout << "Please login now using your registered Credentials\n"<< endl;
		}

	    sqlite3_close(DB);
	}
	else
	{ 
		cout<<"Your Passwords doesn't match. Please try again"<<endl;
		clientObj.warnLog("[WARN] Your Passwords doesn't match. Please try again");
	}
}


/*******************************************************************************
Function Name: userAuthenticationfromDB
Function description: authenticates user
Returns : int
*******************************************************************************/
int DATABASE::userAuthenticationfromDB()
{
	
	string loginUsername; 
	string loginPassword;
	
	sqlite3 *DB;
	char *messaggeError;
	int exit = 0;
	string data("CALLBACK FUNCTION");
		 
	cout << "\n****************************** USER LOGIN PAGE ******************************\n"<<endl;

    cout << "Enter Username :";
    cin >> loginUsername;

    int n=loginUsername.length();
    char char_arr[n+1];
    strcpy(char_arr,loginUsername.c_str());
   
    clientObj.str_trim_lf(char_arr, LENGTH);
    loginPassword = getpass("Enter password :");

    cout << "\n                     ...........Checking................                \n"<<endl;
    exit = sqlite3_open("example.db", &DB);

    if (exit)
    {
        cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
    }

    string sql("SELECT USERNAME FROM registeredUsers WHERE USERNAME = '" + loginUsername + "'");

    int exit3;
    exit3 = sqlite3_exec(DB, sql.c_str(), callbackUsername, (void *)data.c_str(), &messaggeError);
    if (exit3 == 0)
    {
        if (db_username == loginUsername)
        {
            string sql2("SELECT PASSWORD FROM registeredUsers WHERE PASSWORD = '" + loginPassword + "'");
            int exit4;
            exit4 = sqlite3_exec(DB, sql2.c_str(), callbackPassword, (void *)data.c_str(), &messaggeError);
			if(exit4 < 0){
				clientObj.errorLog("[ERROR] unable to execute sql.");
			}
            if (db_password == loginPassword)
            {
                setUserInfo(loginUsername, loginPassword);
                sqlite3_close(DB);
                cout<< "Login Successful!"<<endl; 
				clientObj.infoLog("[INFO] Login Successful");
                return 1;
            }
            else
            {	cout<<"Wrong Password!!"<<endl;
            	cout<<"Please join again with correct Credentials"<<endl;
				clientObj.warnLog("[WARN] Wrong Password");
            	return 0;
            }	
        }
        else
        {
            cout<< "Username doesn't exist!! " << endl;
            cout<< "Please join again with correct Credentials"<<endl;
			clientObj.warnLog("[WARN] Username doesn't exist");
            return 0;
        }
    }
    else
    {
        cout << "Username doesn't exist!!" << endl;
        cout<< "Please join again with correct Credentials"<<endl;
		clientObj.warnLog("[WARN] Username doesn't exist");
        sqlite3_close(DB);
        return 0;
    }
}


/*******************************************************************************
Function Name: sendUsername
Function description: sends username to the socket
Returns : void
*******************************************************************************/
void DATABASE::sendUsername(string usern){
    int n = usern.length();
    char char_array[n + 1];
    strcpy(char_array, usern.c_str());
    int bytesSent = send(c_sockfd, char_array, strlen(char_array), 0);
	if(bytesSent == -1){
		cout<<"ERROR: unable to send."<<endl;
	}
}


/*******************************************************************************
Function Name: setUserInfo
Function description: inserts username and password into the global variable
Returns : void
*******************************************************************************/
void DATABASE::setUserInfo(string loginUsername, string loginPassword){
    send_username = loginUsername;
    send_password = loginPassword;
}


/*******************************************************************************
Function Name: callbackUsername
Function description: extracts the username from database
Returns : int
*******************************************************************************/
int callbackUsername(void *data, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        if (argv[i] != "")
        {
            db_username = argv[i];
            return 0;
        }
        else
            return 1;
    }
    return 1;
}


/*******************************************************************************
Function Name: callbackPassword
Function description: extracts the password from database
Returns : int
*******************************************************************************/
int callbackPassword(void *data, int argc, char **argv, char **azColName){
    for (int i = 0; i < argc; i++)
    {
        if (argv[i] != "")
        {
            db_password = argv[i];
            return 0;
        }
        else
            return 1;
    }
    return 1;
}


/*******************************************************************************
Function Name: getCurrentTime
Function description: returns the current time
Returns : const char *
*******************************************************************************/
const char * CLIENT::getCurrentTime(){
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
void CLIENT::errorLog(char const *LogString){
	client_logFile = fopen("Error_Log.txt", "ab");
	fprintf(client_logFile, "%s, %s", LogString, getCurrentTime());
	fclose(client_logFile);
}


/*******************************************************************************
Function Name: infoLog
Function description: logs infomation into logfile
Returns : void
*******************************************************************************/
void CLIENT::infoLog(char const *LogString){
	client_logFile = fopen("Info_Log.txt", "ab");
	fprintf(client_logFile, "%s, %s", LogString, getCurrentTime());
	fclose(client_logFile);
}
	

/*******************************************************************************
Function Name: warnLog
Function description: logs warnings into logfile
Returns : void
*******************************************************************************/
void CLIENT::warnLog(char const *LogString){
	client_logFile = fopen("Warn_Log.txt", "ab");
	fprintf(client_logFile, "%s, %s", LogString, getCurrentTime());
	fclose(client_logFile);
}