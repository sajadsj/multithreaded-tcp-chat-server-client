/*
 * Title: Multithreaded tcp chat server/client
 * Project aim: learning Multithreading & tcp/ip socket
 * by: SajadSojoudi@gmail.com
 * Client Code
*/
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#include <vector>
#include <istream>
#include "Logindata.h"


#pragma comment(lib, "ws2_32.lib")

//#include <mutex>
using namespace std;


void main()
{
	login_data client_t;
	vector <thread> t_rec;
	transtruct data_to_send, get_data;
	
	SOCKET sock = connect_to_server();
	t_rec.push_back(thread(rec, sock, ref (get_data), ref (client_t)));
	get_com(sock, ref(data_to_send), ref (client_t));

	//cleanupwinsock
	WSACleanup();
	system("pause");
}
