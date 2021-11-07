#ifndef __CLIENTCLASS_H_
#define __CLIENTCLASS_H_


#include <iostream>
#include <WS2tcpip.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <map>
#include "datastruct.h"

using namespace std;

class client_class
{
public:

	struct user_pass_struct
	{
		string user, pass;
	};
	int i = 0;

	vector <user_pass_struct> all_users;
	user_pass_struct key;
	vector <string> online_users;
	map <string, SOCKET> session;
	map <string, string> chat_rq;
	map <string, string> msg_m;
	vector <chat_acc> status;

	//vector <msg_s> msg_vec;

	//functions

	void add_client(string username, string pass);

	// Checks whether the value x is present in users
	bool search(string username, string pass);

	bool repeat_check(string username);

	string get_onlines(string username);

	bool on_or_not(string username);
	
	//SOCKET	find_soc(string targetuser);

	string get_req(string username);

	bool get_acc(string user, string target);

	string get_msg(string user);

};

client_class readData(client_class clinets_class);
string update_the_users(string n_username, string n_pass);
void print_all_users(client_class x);

void data_thread(transtruct tmp, SOCKET current_client, client_class& clients_class, mutex& m);
void receive_cmds(SOCKET current_client, client_class& clients_class, mutex& m);

#endif
