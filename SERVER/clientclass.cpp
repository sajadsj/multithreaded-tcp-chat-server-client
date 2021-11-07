#include <iostream>
#include <WS2tcpip.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <thread>
#include <map>
#include <mutex>

#include "clientclass.h"



using namespace std;
//read data form file
client_class readData(client_class clinets_class)
{
	client_class clients_class;
	// Declare local variables
	string username, pass;

	// Open input file
	ifstream din;
	din.open("./users.txt");
	if (din.fail())
	{
		cerr << "Could not open file: users.txt" << endl;
	}

	cout << "Reading from file..." << endl;

	// Read data
	while (!din.eof())
	{
		din >> username >> pass;


		if (clients_class.repeat_check(username) == false)
			clients_class.add_client(username, pass);
	}
	din.close();
	return clients_class;
}

string update_the_users(string n_username, string n_pass)
{
	// Open output file
	ofstream dout;
	dout.open("users.txt", ios_base::app); // append instead of overwrite

	if (dout.fail())
	{
		return "\nCould not open file: users.txt";
	}

	cout << "Updating database...\n";

	// write data
	dout << endl << n_username << endl << n_pass;

	dout.close();
	return "\nDatabase updated.\n";
}

void print_all_users(client_class x)
{
	for (int g = 0; g < x.all_users.size(); g++)
	{
		cout << "username of " << g << "est user is " << x.all_users[g].user << endl;
		//cout << "pass of" << g << "user is " << x.all_users[g].pass << endl;
	}
}




	//vector <msg_s> msg_vec;

	//functions

	void client_class::add_client(string username, string pass)
	{
		user_pass_struct tmp;
		tmp.user = username;
		tmp.pass = pass;
		all_users.push_back(tmp);
	}

	// Checks whether the value x is present in users
	bool client_class::search(string username, string pass)
	{

		key.user = username;
		key.pass = pass;


		for (int k = 0; k < all_users.size(); k++)
		{
		if (all_users[k].user.compare(username) == 0 && all_users[k].pass.compare(pass) == 0)
			return true;
		}
		return false;
	}


	bool client_class::repeat_check(string username)
	{
		if (all_users.size() == 0)
			return false;

		for (int j = 0; j < all_users.size(); j++)
		{
			
			if (all_users[j].user == username) //the user is in the all user list
				return true;		
		}
		return false;
	}



	string client_class::get_onlines(string username)
	{
		if (online_users.size() == 0)
		{
			return "";
		}

		else
		{
			string online_string = "";
			for (int i = 0; i < online_users.size(); i++)
			{
				//check for the user himself
				if (online_users[i] != username)
				{
					online_string = online_string + online_users[i] + "\n";
				}

			}
			
			return online_string;

		}
	}

	bool client_class::on_or_not(string username)
	{

		//FALSE output means username is online now!
		if (online_users.size() == 0)
		{
			return true;
		}

		else
		{

			for (int i = 0; i < online_users.size(); i++)
			{
				//check for the user
				if (online_users[i] == username)
				{
					return false;
				}

			}
		}
		return true;
			
	}

	/*
	SOCKET	client_class::find_soc(string targetuser)
	{
		map<string, SOCKET>::iterator it;
		it = session.find(targetuser);

		if (it == session.end())
			cout << "Key-value pair not present in map";
		else
		{
			cout << "Key-value pair present : "
				<< it->first << "->" << it->second;
			SOCKET target_soc = it->second;
			return target_soc;
		}
	}
	*/
	
	string client_class::get_req(string username)
	{
		map <string, string> ::iterator it;
		it = chat_rq.find(username);

		if (it == chat_rq.end())
		{
			//cout << "no chat req for you";
			return "";
		}
			
		else
		{
			cout <<"\nget_rec function:  chat req from  "
				<< it->first << " for " << it->second<<endl;
			return it->second;
		}

	}
	
	bool client_class::get_acc(string user, string target)
	{
		if (status.size() == 0)
		{
			return false;
		}

		else
		{
			//string accept = "";
			for (int i = 0; i < status.size(); i++)
			{
				//check for the user himself
				if (status[i].user == user && status [i]. target == target)
				{
					if (status[i].acc == 'y')
						return true;
					//accept = status[i].target + " accepted your chat request. You two can chat in 5 menu";
					else 
						return false;
						// ya darkhst sabt nashode ya ghabul nakarde
						
					//accept = status[i].target + " declined your chat request.";
				}
			
			}	

		}
	}

	string client_class::get_msg(string user)
	{
		map <string, string> ::iterator it;
		it = msg_m.find(user);

		if (it == msg_m.end())
		{
			//cout << "no chat req for you";
			return "";
		}

		else
		{
			cout << "\nget_rec function:  chat req from  "
				<< it->first << " for " << it->second << endl;
			return it->second;
		}

	}


void data_thread(transtruct tmp, SOCKET current_client, client_class& clients_class, mutex& m)
{

	string current_username = tmp.username;
	//client of this thread's username is clients_class.client_username 
	cout << "\nData thread for " << current_username << " started." << endl;


	while (true)
	{
		string chat_reqs, msg_for_me = "";
		m.lock();
		chat_reqs = clients_class.get_req(current_username);
	    msg_for_me = clients_class.get_msg(current_username);
		m.unlock();

		if (chat_reqs != "")
		{
			//302: there is chat req
			tmp.st = 302;

			//chat_reqs = "\nYou have chat request from:\n" + chat_reqs + "\nAnswer with y or n";

			int send_r = send(current_client, reinterpret_cast <char*> (&tmp), sizeof(tmp), 0);


			if (send_r != SOCKET_ERROR)
			{
				send(current_client, chat_reqs.c_str(), chat_reqs.size(), 0);
				m.lock();
				clients_class.chat_rq.erase(current_username);
				m.unlock();
			}
		}

		//msging

		if (msg_for_me != "")
		{
			tmp.st = 501; //501 this is msg from other client(s)

			int send_r = send(current_client, reinterpret_cast <char*> (&tmp), sizeof(tmp), 0);

			if (send_r != SOCKET_ERROR)
			{

				int send_msg = send(current_client, msg_for_me.c_str(), msg_for_me.size(), 0);
				cout << "\n data threade " << current_username << msg_for_me << endl;
				if (send_msg != SOCKET_ERROR)
				{
					m.lock();
					//delet the msg
					clients_class.msg_m.erase(current_username);
					m.unlock();
				}
			}
		}
	}

}


void receive_cmds(SOCKET current_client, client_class& clients_class, mutex& m)
{
	printf("thread created.\r\n");
	transtruct tmp;
	int sendResult = 0;

	vector <thread> t_data;
	chat_acc my, my1;
	msg_s my_msg;
	string msg = "";
	char buf_c[4096];
	ZeroMemory(buf_c, 4096);
	SOCKET target_sock;
	bool target_rep, target_on;
	int recmsg;
	// our recv loop
	while (true)
	{
		string online_refresh, online_string, chat_reqs, chat_acc, select_target = "";
		//chec for chat request, if there was any, request for answer


		//wait for command
		int bytesIn = recv(current_client, reinterpret_cast <char*> (&tmp), sizeof(tmp), 0);

		Sleep(10);
		//ERROR
		if (bytesIn <= 0)
		{
			//Drop the client
			closesocket(current_client);
			ExitThread(0);
		}
		else
		{


			//check the command
			switch (tmp.com)
			{
				//Login to server
			case 1:

				if (clients_class.search(tmp.username, tmp.pass) == true && clients_class.on_or_not(tmp.username) == true)
				{

					string welcomeMsg = "";
					//login was succecefull

					//if online_users are empty, say you are alone else send him/her online_users
					m.lock();
					//if there is no one online in the server
					if (clients_class.online_users.size() == 0)
					{
						//welcomeMsg = "Hellow " + tmp.username + ", welcome to chat server!\nSadly you are alone in server!\n";
						tmp.st = 101; //login succecfuland no one is online
					}
					//there are some onlines
					else
					{
						//102: login succecful and here is the onlines
						tmp.st = 102;
						online_string = clients_class.get_onlines(tmp.username);
						//welcomeMsg = "Hellow " + tmp.username + ", welcome to chat server!\nOnlines are:\n" + online_string;
					}

					//add user to online users
					clients_class.online_users.push_back(tmp.username);
					//Creat a session
					clients_class.session.insert(pair<string, SOCKET>(tmp.username, current_client));
					m.unlock();

					//data thread start
					t_data.push_back(thread(data_thread, tmp, current_client, ref(clients_class), ref(m)));
				}
				else
				{
					//user pass not match, or already loged in
					tmp.st = 104;
					//tmp.s_msg = "There is something wrong with your username or password \nor you already loged in!";
				}

				send(current_client, reinterpret_cast <char*> (&tmp), sizeof(tmp), 0);
				Sleep(10);

				if (tmp.st == 102)
				{
					//send onlines
					send(current_client, online_string.c_str(), online_string.size(), 0);
					Sleep(10);
				}
				break;




				//Register to server
			case 2:

				if (clients_class.repeat_check(tmp.username) == false)
				{
					clients_class.add_client(tmp.username, tmp.pass);
					cout << tmp.username << " added to server." << endl;

					//201: register succes
					tmp.st = 201; //this is a server responce
					//tmp.s_msg = "You are added to server.";

					//send(current_client, "You are added to server.", 25, 0);

					cout << update_the_users(tmp.username, tmp.pass);
				}
				else
				{
					tmp.st = 204;
					//tmp.s_msg = "Username is repeated!";
				}
				send(current_client, reinterpret_cast <char*> (&tmp), sizeof(tmp), 0);
				break;

				//Refresh online list & Get chat reqs & status
			case 3:
				//first reset online list
				online_refresh = "";

				m.lock();
				online_refresh = clients_class.get_onlines(tmp.username);
				m.unlock();

				//some onlines in here		//there was a chat req from you
				if (online_refresh != "")
				{
					tmp.st = 301;		//here is onlines and chat req result if there was any
				}
				else
				{
					tmp.st = 303; //303: there are not online
				}

				send(current_client, reinterpret_cast <char*> (&tmp), sizeof(tmp), 0);


				if (tmp.st == 301)
				{
					//send onlines
					send(current_client, online_refresh.c_str(), online_refresh.size(), 0);
					Sleep(10);
				}
				break;





				//Select target user for chat
			case 4:
				my1 = {};
				my1.acc = 'y';
				my1.user = tmp.username;
				my1.target = tmp.targetuser; //az target user maloom mishavad be kodam request yes dade ast.

				m.lock();
				target_rep = clients_class.repeat_check(tmp.targetuser);
				target_on = clients_class.on_or_not(tmp.targetuser);
				m.unlock();

				if (tmp.targetuser == tmp.username)
				{
					tmp.st = 405; //405: you can not req to yourself
				}
				else
				{

					//check if target user registered in to server
					if (target_rep == false)
					{

						tmp.st = 403; //403: there is not such user in the server
					}
					else
					{
						//chek if he is online
						if (target_on == false)
						{
							//he is online		

							//push to chat reqs queue
							m.lock();
							clients_class.chat_rq.insert(pair<string, string>(tmp.targetuser, tmp.username));
							clients_class.status.push_back(my1);
							//push status accept
							m.unlock();


							tmp.st = 401; //401: your req submited.

						}
						else
						{
							//he is not online
							//select_target = "Your target user is not online\n";
							tmp.st = 404; //404: your selected user is not online
						}
					}


				}
				send(current_client, reinterpret_cast <char*> (&tmp), sizeof(tmp), 0);
				break;


				//chating service
			case 5:
				 msg = "";
				recmsg = recv(current_client, buf_c, 4096, 0);
				if (recmsg > 0)
				{

					 msg = tmp.username + " : " + string(buf_c);

					//push msg for target user
					m.lock();
					if (clients_class.get_acc(tmp.username, tmp.targetuser) == true)
					{
						clients_class.msg_m.insert(pair <string, string>(tmp.targetuser, msg));
					}
					m.unlock();

				}

				break;

				//answer to chat req
			case 6:
				my.acc = tmp.acc;
				my.user = tmp.username;
				my.target = tmp.targetuser; //az target user maloom mishavad be kodam request yes dade ast.

				m.lock();
				clients_class.status.push_back(my);
				m.unlock();

				if (tmp.acc == 'n')
				{
					m.lock();
					//del chat req
					clients_class.chat_rq.erase(tmp.username);
					m.unlock();
				}
				break;

			default:
				cerr << "wrrong command from client!" << endl;
				//Drop the client
				closesocket(current_client);
				//drop socket

			}
			cout << "\ntmp com is : " << tmp.com << endl;
			cout << "tmp user is : " << tmp.username << endl;
			cout << "tmp target user is : " << tmp.targetuser << endl;
		}

	}
}

