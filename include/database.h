#ifndef DATABASE_H
	#define DATABASE_H


/*******************************************************************************
CLASS PROTOTYPE
*******************************************************************************/
class DATABASE{
	
	public:
	
	void userRegisterationtoDB();
	void setUserInfo(string loginUsername, string loginPassword);
	int userAuthenticationfromDB();
	void sendUsername(string usern);
};


#endif