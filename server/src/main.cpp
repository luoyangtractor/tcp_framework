#include "Worker_Thread_Manager.h"
#include <iostream>
#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib")  

int main()
{
	WORD sockVersion = MAKEWORD(2, 2);

	WSADATA wsaData;

	if (WSAStartup(sockVersion, &wsaData) != 0)

	{

		return 0;

	}
	int i;
	std::cin >> i;
	return 0;
}
