/*
 * Title: Multithreaded tcp chat server/client
 * Project aim: learning Multithreading & tcp/ip socket
 * by: SajadSojoudi@gmail.com
 * Server Code
*/
#include <iostream>
#include <WS2tcpip.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

#pragma comment (lib, "WS2_32.lib")

#include "clientclass.h"



using namespace std;

int main()

{
	mutex m;
	client_class clients_class;
	transtruct my;

	clients_class = readData(clients_class);
	if (clients_class.all_users.size() > 0)
		cout << "data added from file." << endl;
	else
		cout << "data did NOT added from file!" << endl;


	print_all_users(clients_class);
	cout << "data printed" << endl;


	//Initialize winsock
	WSADATA wsData;
	WORD ver = (2, 2);
	int wsok = WSAStartup(ver, &wsData);

	//ERROR
	if (wsok != 0)
	{
		cerr << "\nCan't initialize wisock! Quitting..." << endl;
		return -1;
	}

	//Creat our masterSocket(socket that listens for connections)
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

	//ERROR
	if (listening == INVALID_SOCKET)
	{
		cerr << "\nCan't creat a socket! Quiting..." << endl;
		return -1;
	}

	else
		cout << "Server is running..." << endl;



	//Bind an IP address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //could also use inet_ptone

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	cout << "IP binded to socket." << endl;

	//Tell winsock the socket is for lisstening
	if (listen(listening, SOMAXCONN) != 0)
		cerr << "Listening ERROR!" << endl;


	
	// socket that we snedzrecv data on
	SOCKET client_socket;

	sockaddr_in from;
	int fromlen = sizeof(from);

	vector <thread> t_com;
	
	// loop forever 
	while (true)
	{
		// Accept connections
		client_socket = accept(listening, (struct sockaddr*)&from, &fromlen);
		cout << "Client connected" << endl;
		t_com.push_back(thread(receive_cmds, client_socket, ref(clients_class), ref (m)));
	
	}



	//cleanupwinsock
	WSACleanup();
	system("pause");
}