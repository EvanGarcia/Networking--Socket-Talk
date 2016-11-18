//Evan Garcia
//CSC 3350
//Lab 6
//June 5, 2016
//SERVER.c

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HOSTNAMELEN 128
#include <stdio.h>
#include <winsock2.h>
#include <conio.h>

int main(int argc, char *argv[])
{
	WORD wVersionRequested; 
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(wVersionRequested, &wsaData) != 0) //Initialize socket package
	{
		fprintf(stderr, "Process failed on WinSock startup\n");
		ExitProcess(0);
	};

	SOCKET skt = socket(AF_INET, SOCK_STREAM, 0);

	char hostName[HOSTNAMELEN];
	gethostname(hostName, HOSTNAMELEN);  //Get hostname
	LPHOSTENT host = gethostbyname(hostName); //Set hostname
	u_short portNumber = 0; //Get random port number

	SOCKADDR_IN hostReference;
	ZeroMemory(&hostReference, sizeof(SOCKADDR_IN));
	hostReference.sin_family = AF_INET;
	hostReference.sin_port = htons((u_short)portNumber);
	CopyMemory(&hostReference.sin_addr, host->h_addr_list[0], host->h_length);
	
	printf("Server Host Name:  %s\n", hostName); //Print host name

	int result = bind(skt, (const SOCKADDR *) &hostReference, sizeof(hostReference));

	int errorNumber = GetLastError();

	int len = sizeof(hostReference);

	getsockname(skt, (SOCKADDR *)&hostReference, &len);

	u_short actual_port = ntohs(hostReference.sin_port);
	printf("Server Port Number: %d\n", actual_port); //Print server port number

	int ifListening = listen(skt, SOMAXCONN); //Listen for connection

	if (ifListening == -1)
	{
		printf("Error attempting to connect to client...\n");
	}
	else
	{
		printf("Listening...\n");
	}

	SOCKADDR_IN Client;
	int ClientLen = sizeof(SOCKADDR_IN);

	SOCKET CONNSkt;

	CONNSkt = accept(skt, (SOCKADDR *)&Client, &ClientLen); //Attempt to accept connection
	int ifaccepted = CONNSkt;
	if (ifListening == -1)
	{
		printf("Error attempting to connect to client...\n");
		return 0;
	}
	else
	{
		printf("Accepted connection request...\n");
	}

	closesocket(skt);

	unsigned long mode = 1;
	ioctlsocket(CONNSkt, FIONBIO, &mode); //Set to nonblocking
	
	int flags = 0;
	
	//int bytes_received = 0;

	char sendBuff[BUFSIZ];
	char receiveBuff[BUFSIZ];
	
	printf(">");
	for(;;)
	{
		if (_kbhit())
		{
			gets_s(sendBuff, BUFSIZ);

			if (strcmp(sendBuff, "exit") == 0 || strcmp(sendBuff, "EXIT") == 0) //Exit if user inputs "exit"
			{
				send(CONNSkt, sendBuff, strlen(sendBuff), 0);
				printf("\nExiting...\n\n");
				closesocket(CONNSkt); //Close socket
				WSACleanup(); //Release  all network socket-related resources
				return 0;
			}
			else {
				send(CONNSkt, sendBuff, strlen(sendBuff), 0); //If user doesn't input "exit" then send input to CLIENT
				printf("\n>");
			}
		}
		int bytes_received = recv(CONNSkt, receiveBuff, BUFSIZ, 0); 
		if ( bytes_received > 0) //If received data from CLIENT, then enter statement
		{
			receiveBuff[bytes_received] = '\0';

			if (strcmp(receiveBuff, "exit") == 0 || strcmp(receiveBuff, "exit") == 0) //If received "exit" then end program
			{
				printf("\r<%s\n\n", receiveBuff);
				printf("Exiting...\n\n");
				closesocket(CONNSkt); //Close socket
				WSACleanup(); //Release  all network socket-related resources
				return 0;
			}
			else
			{
				printf("\r\t\t\t"); //If user didn't enter "exit" then print received information
				printf("\r<%s \n", receiveBuff);
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