// WSJTXLogger32Relay.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include <sysinfoapi.h>
//#include <windows.h>
//#include <minwinbase.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
//#include "hexdump.h"

#ifdef __cplusplus
extern "C" {
#endif

int InitWinsock()
{
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

int main(int argc, char* argv[])
{
	SOCKET socketS;
	
	int err = 0;
	
	struct sockaddr_in localS;
	struct sockaddr_in from;
	struct sockaddr_in to;
	
	SYSTEMTIME time_is_nigh;
	int messages_this_slot = 0;
	int slot = 0;
	int currentslot = 0;
	
	int fromlen = sizeof(from);
	int tolen = sizeof(to);


	err = InitWinsock();
	if (err != 0)
		return err;

	// Listen on 0.0.0.0:2240
	localS.sin_family = AF_INET;
	localS.sin_port = htons(2240);
	localS.sin_addr.s_addr = INADDR_ANY;
	//localS.sin_addr.s_addr = htonl(0xc0a80dc6); // 192.168.13.198 for testing
	//localS.sin_addr.S_un.S_addr = INADDR_ANY;
	
	// Send back to localhost:2237
	to.sin_family = AF_INET;
	to.sin_port = htons(2237);
	to.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
	//to.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
	//to.sin_addr.s_addr = htonl(0xc0a80d09); // 192.168.13.9 for testing
	//to.sin_addr.s_addr = htonl(0xc0a80dc6); // 192.168.13.198 for testing
	// @TODO: Add option for listening and target IP and ports as arguments, maybe even multiple targets?
	// @TODO: Add return relay (eg. for controlling WSJT-X from Logger32)


	socketS = socket(AF_INET, SOCK_DGRAM, 0);
	bind(socketS, (sockaddr*)&localS, sizeof(localS));
	printf("Waiting for messages...\n");
	while (1)
	{
		char buffer[SO_MAX_MSG_SIZE];
		ZeroMemory(buffer, sizeof(buffer));
		err = 0;
		err = recvfrom(socketS, buffer, sizeof(buffer), 0, (sockaddr*)&from, &fromlen);
		//err = recv(socketS, buffer, sizeof(buffer), 0);
		if (err != SOCKET_ERROR && err != INVALID_SOCKET && err > 0)
		{
			
			
			GetSystemTime(&time_is_nigh);
			if (time_is_nigh.wSecond >= 55 || time_is_nigh.wSecond < 10)
			{
				slot = 0;
			}
			else if (time_is_nigh.wSecond >= 10 && time_is_nigh.wSecond < 25)
			{
				slot = 1;
			}
			else if (time_is_nigh.wSecond >= 25 && time_is_nigh.wSecond < 40)
			{
				slot = 2;
			}
			else
			{
				slot = 3;
			}

			if (currentslot != slot)
			{
				messages_this_slot = 0;
				currentslot = slot;
				printf("\r\n");
			}
			
			messages_this_slot++;
			
			printf("\r%04d-%02d-%02d %02d:%02d:%02d Decodes slot %d: %d", time_is_nigh.wYear, time_is_nigh.wMonth, time_is_nigh.wDay, time_is_nigh.wHour, time_is_nigh.wMinute, time_is_nigh.wSecond, slot, messages_this_slot);
			//printf(".");
			
			sendto(socketS, (const char*)&buffer, err, 0, (sockaddr*)&to, tolen);
		}
		
		// Don't hog...
		Sleep(0);
	}
	
	closesocket(socketS);

	return 0;
}

#ifdef __cplusplus
}
#endif
