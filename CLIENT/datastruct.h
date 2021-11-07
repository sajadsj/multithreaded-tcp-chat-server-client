#ifndef _DATASTRUCT_H_
#define _DATASTRUCT_H_

#include <string>


struct transtruct      //transfer_strcut
{
	//control
	int com;  //1 for login, 2 for register, 3 chat request
	std::string username, pass;
	std::string targetuser;
	char acc;

	//data
	//char msg[150];
	int st; // 1: this is a chat request, 2: this is a server respons, 3: this is a msg from other clients, 4: ERROR
};


#endif
