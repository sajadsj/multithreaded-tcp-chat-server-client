#include "logindata.h"
#include <iostream>
#include <WS2tcpip.h>
#include <thread>

using namespace std;

//connect to socket

SOCKET connect_to_server()
{
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return SOCKET_ERROR;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return SOCKET_ERROR;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return SOCKET_ERROR;
	}


	return sock;

	//return SOCKET();
}

	transtruct login_data::login_to_server()
	{
		client.com = 1;
		client.targetuser = "";
		client.st = 0;
		client.acc = ' ';
		cout << "logining to server..." << endl;
		cout << "Enter your username:" << endl;
		cin >> client.username;
		cout << "Enter your pass:" << endl;
		cin >> client.pass;
		return client;
	}

	transtruct login_data::register_to_server()
	{

		string con_pass;
		client.com = 2;
		client.targetuser = "";
		cout << "registering to server..." << endl;
		cout << "Enter a username:" << endl;
		cin >> client.username;
		cout << "Enter a pass:" << endl;
		cin >> client.pass;
		cout << "confirm your pass:" << endl;
		cin >> con_pass;

		while (true)
		{
			if (con_pass.compare(client.pass) == 0)
				break;

			else
			{
				cerr << "pass are not matched!" << endl;
				cout << "Enter a pass:" << endl;
				cin >> client.pass;
				cout << "confirm your pass:" << endl;
				cin >> con_pass;
			}
		}
		return client;

	}

	transtruct login_data::select_target_user()
	{

		client.com = 4;
		cout << "Selecting a user for chat..." << endl;
		cout << "Enter a username:" << endl;
		cin >> client.targetuser;
		return client;
	}

	transtruct login_data::chat_rq_ans()
	{
		client.com = 6;
		//answer to chat request
		do {
			cout << "Do you want to chat? (Please answer with y or n only)" << endl;
			cin >> client.acc;
			if (client.acc == 'y')
			{
				cout << "\nConfirm your target user:" << endl;
				cin >> client.targetuser;
				cout << "\nGo to chat using 5 in menu." << endl;
				//return client;
			}
			else if (client.acc == 'n')
			{
				cout << "Your answer saved" << endl;
				//return client;
			}
			else
			{
				cout << "Please try again. answer with y or n only." << endl;
				client.targetuser = "";
				client.acc = ' ';

			}

		} while (client.acc != 'n' && client.acc != 'y');
		return client;
	}

	//rec thread
	void rec(SOCKET sock, transtruct& data_to_send, login_data& client_t)
	{
		char buf[4096];
		ZeroMemory(buf, 4096);
		string msg = "";
		while (true)
		{
			int bytesReceived = recv(sock, reinterpret_cast <char*> (&data_to_send), sizeof(data_to_send), 0);
			//recv(sock, buf, 4096, 0);
			Sleep(10);


			if (bytesReceived > 0)
			{
				//cout << ">SERVER: " << data_to_send.msg;

				// 1: this is a chat request, 2: this is a server respons, 3: this is a msg from other clients, 4: ERROR
				switch (data_to_send.st)
				{

				case 101://login succecful and no one is online	
				{
					cout << "\nHellow " << data_to_send.username << ", welcome to chat server!\nSadly you are alone in server!\n";
				}
				break;

				case 102://102: login succecful and here is the onlines
				{
					cout << "Hellow " << data_to_send.username << ", welcome to chat server!\n" << endl;
					cout << "Here is online users:\n";
					//recv

					int bytesReceived = recv(sock, buf, 4096, 0);
					if (bytesReceived > 0)
					{
						cout << string(buf);
					}

				}
				break;

				//104: user pass not match, or already loged in
				case 104:
				{
					cout << "\nThere is something wrong with your username or password \nor you already loged in!" << endl;
				}
				break;

				//201: register succes
				case 201:
				{
					cout << "You are added to server." << endl;
				}
				break;

				//204: username is repeated
				case 204:
				{
					cout << "Username is repeated" << endl;
				}
				break;


				//here is onlines and chat req result if there was any
				case 301:
				{
					cout << "Here is online users and chat request results if you had any:\n";

					//recv
					int bytesReceived = recv(sock, buf, 4096, 0);
					if (bytesReceived > 0)
					{
						cout << string(buf);
					}

				}
				break;

				//303: there are not online
				case 303:
				{
					cout << "There are not any online user.\n" << endl;
				}
				break;


				//302: there is chat req
				case 302:
				{
					//popup
					cout << "You have chat request(s) notification, ";

					int bytesReceived = recv(sock, buf, 4096, 0);
					if (bytesReceived > 0)
					{
						cout << "from: \t" << string(buf) << "\nPlease answer it by typing 6 in menu." << endl;
					}
					Sleep(10);
					memset(buf, 0, 4096);
				}
				break;


				case 401:
				{
					cout << "\nYour chat request submited." << endl;
				}
				break;

				case 403:
				{
					cout << "Sorry, there is not " << data_to_send.targetuser << " in the server." << endl;
				}
				break;

				case 404:
				{
					cout << "\nYour target user is not online." << endl;
				}
				break;

				//405: you can not req to yourself
				case 405:

					cout << "You can not req to yourself!" << endl;
					break;


					//501 this is msg from other client(s)
				case 501:

					int bytesReceived = recv(sock, buf, 4096, 0);
					if (bytesReceived > 0)
					{
						msg = string(buf) + "\n";
					}
					cout << msg << endl;
					//m.lock();
					//client_t.chats.push_back(msg);
					//m.unlock();
					break;
				}
			}
		}
	}

	//get commands
	void get_com(SOCKET sock, transtruct& data_to_send, login_data& client_t)
	{

		// get client commands ui
	
		string userInput;

		int sendResult;


		cout << "SOCKET initiated." << endl << "\nPlease login to server" << endl;

		char buf_c[4096];
		ZeroMemory(buf_c, 4096);

		//char target[20], msg[1024];

		string msg;
		int login_error = 0;
		int ux;
		string delimiter = ",";



		while (true)

		{
			cout << "1: Login, 2: Register, 3: Refresh online users, 4: select user to request, 5: Go to chats 6: Answer to chat req 9: Exit"
				<< endl;
			cin >> ux;

			switch (ux)
			{
				//Login 
			case 1:
				data_to_send = client_t.login_to_server();
				break;
				//Register
			case 2:
				data_to_send = client_t.register_to_server();
				break;

				//Refresh online users
			case 3:


				if (data_to_send.username == "" || data_to_send.pass == "")
				{
					cout << "Pleas login first!" << endl;
					login_error = -1;
					break;
				}
				else
					data_to_send.com = 3;

				break;
				//Select a user for chat
			case 4:
				if (data_to_send.username == "" || data_to_send.pass == "")
				{
					cout << "Pleas login first!" << endl;
					login_error = -1;
					break;
				}
				else
					data_to_send = client_t.select_target_user();
				break;


			case 5:


				if (data_to_send.username == "" || data_to_send.pass == "")
				{
					cout << "Pleas login first!" << endl;
					login_error = -1;
					break;
				}
				else
				{
					cout << "\t\tEnter you'r msg in this format (targetuser, your msg...) > "<<endl;
					getline(cin, userInput);
					do
					{
						userInput = "";
						data_to_send.com = 5;

						//m.lock();
						//client_t.print_chat();
						//m.unlock();

						getline(cin, userInput);

						if (userInput.size() > 0)
						{
							if (userInput == "q")
							{
								break;
							}
							else
							{	//find target user by serching , charecter wich defferc msg and target name
								data_to_send.targetuser = userInput.substr(0, userInput.find(delimiter));

								msg = userInput.substr(userInput.find(delimiter) + 1);


								int sendResult = send(sock, reinterpret_cast <char*> (&data_to_send), sizeof(data_to_send), 0);
								Sleep(10);

								if (sendResult != SOCKET_ERROR)
								{
									send(sock, msg.c_str(), msg.size(), 0);
								}
								else
									cerr << "Send ERROR!" << endl;
							}

						}

					} while (userInput.size() > 0);

				}
				break;


			case 6:

				if (data_to_send.username == "" || data_to_send.pass == "")
				{
					cout << "Pleas login first!" << endl;
					login_error = -1;
					break;
				}
				else
					data_to_send = client_t.chat_rq_ans();
				break;

			case 9:
				exit(0);
				break;


			default:
				cout << "Worrong command!" << endl;
				login_error = -1;
			}



			if (ux != 5 && login_error == 0)
			{
				sendResult = send(sock, reinterpret_cast <char*> (&data_to_send), sizeof(data_to_send), 0);
				//Sleep(10);
				//recv(sock, reinterpret_cast <char*> (&data_to_send), sizeof(data_to_send), 0);
				if (sendResult == SOCKET_ERROR)
				{
					cerr << "Send ERROR!" << endl;
				}
			}

		}
	}