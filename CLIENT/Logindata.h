
#ifndef __LOGINDATA_H_
#define __LOGINDATA_H_

#include "datastruct.h"
#include <WS2tcpip.h>



SOCKET connect_to_server();

class login_data
{
public:
	transtruct client;
	//vector <string> chats;

	transtruct login_to_server();

	transtruct register_to_server();

	transtruct select_target_user();

	transtruct chat_rq_ans();

	/*
	void print_chat();

	*/

};

void rec(SOCKET sock, transtruct& data_to_send, login_data& client_t);

void get_com(SOCKET sock, transtruct& data_to_send, login_data& client_t);

#endif