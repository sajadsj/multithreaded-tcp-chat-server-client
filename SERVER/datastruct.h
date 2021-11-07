#ifndef _DATASTRUCT_H_
#define _DATASTRUCT_H_

#include <string>
using namespace std;

struct transtruct      //transfer_strcut
{
	//control
	int com;  //1 for login, 2 for register, 3 chat request
	string username, pass;
	string targetuser;
	char acc;

	//data
	//string s_msg;
	int st; // 1: this is a chat request, 2: this is a server respons, 3: this is a msg from other clients, 4: ERROR
	//100: login    200: register   300: refresh and chat req    400: target user    500: chat
	//001: succes mood 1   002: succes mood 2	003: fail mood 1	004: fail mood 2		005: fail mode 3


	//101: login succecful and no one is online
	//102: login succecful and here is the onlines 
	//104: user pass not match, or already loged in

	//201: register succes
	//204: username is repeated


	//301: there are onlines and/ or chat req result	303: there is not online		

	//401: your req submited.	403: there is not such user in the server 404: your selected user is not online		405: you can not req to yourself

	//501 this is msg from other client(s)

};


struct chat_acc
{
	string user;
	string target;
	char acc;
};

struct msg_s
{
	string to, msg;
};

#endif