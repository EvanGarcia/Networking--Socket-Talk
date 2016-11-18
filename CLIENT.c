//Evan Garcia
//CSC 3350
//Lab 6
//June 5, 2016
//CLIENT.c

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HOSTNAMELEN 128

#include <stdio.h>
#include <winsock2.h>
#include <conio.h>

int main(int argc, char *argv[])
{

	if (argc > 1 && argc < 4)
	{		WORD wVersionRequested;
		WSADATA wsaData;

		wVersionRequested = MAKEWORD(2, 2);

		if (WSAStartup(wVersionRequested, &wsaData) != 0)
		{
			fprintf(stderr, "Process failed on WinSock startup\n");
			ExitProcess(0);
		};		char hostName[HOSTNAMELEN];		if (argc == 2)		{
			gethostname(hostName, HOSTNAMELEN);		}
		if (argc == 3)
		{
			strcpy(hostName, argv[2]);
		}


		LPHOSTENT host = gethostbyname(hostName);

		u_short portNumber = atoi(argv[1]);

		SOCKADDR_IN theSrvr;
		ZeroMemory(&theSrvr, sizeof(SOCKADDR_IN));
		theSrvr.sin_family = AF_INET;
		theSrvr.sin_port = htons((u_short)portNumber);
		CopyMemory(&theSrvr.sin_addr, host->h_addr_list[0], host->h_length);

		printf("Server Host Name:  %s\n", hostName);

		printf("Server Port Number: %s\n", argv[1]);
		SOCKET CONNSkt = socket(AF_INET, SOCK_STREAM, 0);

		printf("Making connection request...\n");
		int result = connect(CONNSkt, (const SOCKADDR *)&theSrvr, sizeof(theSrvr));

		if (result == -1)
		{
			printf("Error attempting to connect to server...\n");
			return 0;
		}
		else
		{
			printf("Connected...\n");
		}
		int errornumber = GetLastError();

		unsigned long on = 1;
		ioctlsocket(CONNSkt, FIONBIO, &on);
		int flags = 0;

		char receiveBuff[BUFSIZ];
		char sendBuff[BUFSIZ];

		int bytes_received = 0;
		printf(">");
		for (;;)
		{

			if (_kbhit())
			{
				gets_s(sendBuff, BUFSIZ);

				if (strcmp(sendBuff, "exit") != 0 && strcmp(sendBuff, "EXIT") != 0)
				{
					send(CONNSkt, sendBuff, strlen(sendBuff), 0);
					printf("\n>");
				}
				else
				{
					send(CONNSkt, sendBuff, strlen(sendBuff), 0);
					printf("\nExiting...\n\n");
					closesocket(CONNSkt);
					WSACleanup();
					return 0;
				}
			}

			int bytes_received = recv(CONNSkt, receiveBuff, BUFSIZ, 0);
			if (bytes_received > 0)
			{
				receiveBuff[bytes_received] = '\0';

				if (strcmp(receiveBuff, "exit") == 0 || strcmp(receiveBuff, "EXIT") == 0)
				{
					printf("\r<%s\n\n", receiveBuff);
					printf("Exiting...\n\n");
					closesocket(CONNSkt);
					WSACleanup();
					return 0;
				}

				else
				{
					printf("\r\t\t\t");
					printf("\r<%s\n", receiveBuff);
					printf("\n>");
				}

			}

			Sleep(500);
		}
		printf("\n");

		closesocket(CONNSkt);

		WSACleanup();

		return 0;
	}
	else {
		printf("Please enter the proper amount of command line arguments...");
		return 0;
	}
}