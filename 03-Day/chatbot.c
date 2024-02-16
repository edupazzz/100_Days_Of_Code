#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


/*********************************************************************/
/**************************** DEFINES ********************************/
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "15000"
#define FALSE 0
#define TRUE 1

/*********************************************************************/
/**************************** PROTOTYPES *****************************/
void send_data(const unsigned char, size_t, SOCKET);
int parse_data(unsigned char, int);



/*********************************************************************/
/**************************** MAIN ***********************************/
int main()
{
/********************************/
 /********** VARIABLES ***********/
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
					 *ptr = NULL,
					 hints;
					 
	unsigned char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	
	// The handshake initial request
	const unsigned char buff_handshake_p1[] =
	{ 0x00, 0x00, 0x00, 0x00 };
	
	const unsigned char version[] = "[version]\nversion=\"1.14.9\"\n[/version]";	
	const unsigned char name[] = "[login]\nusername=\"ChatBot\"\n[/login]";
	const unsigned char first_message[] = "[message]\nmessage=\"ChatBot connected\"\nroom=\"lobby\"\nsender=\"ChatBot\"\n[/message]";
	
	
/************************************************************************/
/***************************** SETUP THINGS *****************************/

	// initiate use of the Winsock DLL
	// iResult is zero if it's sucessful, or an error code number
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	
	// Filling a block of memory -'hints'- with zeros
	ZeroMemory(&hints, sizeof(hints));
	// defining network parameters
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	// Provides protocol-independent translation 
	// from an ANSI host name to an address
	// iResult is zero if it's sucessful, or an error code number
	iResult = getaddrinfo("127.0.0.1", "15000", &hints, &result);
	if(iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		return 1;
	}
	
	// 'ptr' struct receives the result struct
	ptr = result;
	
	// create the socket to connections
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if(ConnectSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	
	// Connect the socket with the server
	// iResult is zero if it's sucessful, or an error code number
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if(iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket); // close the socket
		ConnectSocket = INVALID_SOCKET; // update the 'ConnectSocket' value
		printf("\nFailed trying to establish connection\n\tconnect function\n");
	}
	
	freeaddrinfo(result);
	
	if(ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	
/************************************************************************/
/************************* SEND and RECEIVE data ************************/

	// send the first handshake to the server
	iResult = send(ConnectSocket, (const char*)buff_handshake_p1, 
	               (int)sizeof(buff_handshake_p1), 0);
	// show the amount of bytes sent
	printf("Bytes Sent: %ld\n", iResult);

	// receiving data from the server and print it
	iResult = recv(ConnectSocket, (char*)recvbuf, recvbuflen, 0);
	printf("Bytes received: %ld\n", iResult);
	
	// send game version to the server
	send_data(version, sizeof(version), ConnectSocket);
	// receiving response from the server
	iResult = recv(ConnectSocket, (char*)recvbuf, recvbuflen, 0);
	printf("Bytes received: %ld\n", iResult);

	// send the name of the user: ChatBot
	send_data(name, sizeof(name), ConnectSocket);

	// send the first message: "'name' connected"
	send_data(first_message, sizeof(first_message), ConnectSocket);

	
	do{
		iResult = recv(ConnectSocket, (char*)recvbuf, recvbuflen, 0);
		if(iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
		}
		else if(iResult == 0)
		{
			printf("Connection closed\n");
		}
		else{
			printf("recv failed with error: %d\n", WSAGetLastError());
		}
		
		if(parse_data(recvbuf, iResult))
		{
			const unsigned char message[] = "[message]\nmessage=\"Hello!\"\nroom=\"lobby\"\nsender=\"ChatBot\"\n[/message]";
			send_data(message, sizeof(message), ConnectSocket);
		}
	}while(iResult > 0);

	closesocket(ConnectSocket);
	WSACleanup();
	
	return 0; // End of MAIN
}


/*********************************************************************/
/*********************** SEND DATA FUNCTION **************************/

void send_data(const unsigned char *data, size_t len, SOCKET s)
{// compress the data and sent it to server wtf???
	gzFile temp_data = gzopen("packet.gz", "wb"); // new packet
	gzwrite(temp_data, data, len); // writing "data" value to it
	gzclose(temp_data); // closing the file
	
	// open the packet
	FILE* temp_file = NULL;
	fopen_s(&temp_file, "packet.gz", "rb");
	
	if(temp_file)
	{
		size_t compress_len = 0;
		unsigned char buffer[DEFAULT_BUFLEN] = { 0 };
		
		// read from the packet; 'buffer' receives the data from 'temp_file'
		// 'compress_len' is equal to number of items read
		compress_len = fread(buffer, 1, sizeof(buffer), temp_file);
		fclose(temp_file); // closing the file


		/* buffer that will receives our definitive packet */
		unsigned char buff_packet[DEFAULT_BUFLEN] = { 0 };
		// set the begin of the packet with the size of the packet
		memcpy(buff_packet + 3, &compress_len, sizeof(compress_len));
		// pass the compress data to the rest of the packet
		memcpy(buff_packet + 4, buffer, compress_len);
		
		int iResult = send(s, (const char*)buff_packet, compress_len+4, 0);
		printf("Bytes Sent: %ld\n", iResult);		
	}
}
	
	
int parse_data(unsigned char *buff, int buff_len)
{
	unsigned char data[DEFAULT_BUFLEN] = { 0 };
	memcpy(data, buff + 4, buff_len - 4);
	
	FILE* temp_file = NULL;
	fopen_s(&temp_file, "packet_recv.gz", "wb");
	
	if(temp_file)
	{
		fwrite(data, 1, sizeof(data), temp_file);
		fclose(temp_file);
	}
	
	gzFile temp_data_in = gzopen("packet_recv.gz", "rb");
	unsigned char decompressed_data[DEFAULT_BUFLEN] = { 0 };
	
	gzread(temp_data_in, decompressed_data, DEFAULT_BUFLEN);
	fwrite(decompressed_data, 1, DEFAULT_BUFLEN, stdout);
	gzclose(temp_data_in);
	
	char* ret_value;
	
	// receives the return value
	ret_value = strstr((const char*)decompressed_data, (const char*)"\\wave");
	
	if(ret_value == NULL)
	{
		return FALSE;
	}else{
		return TRUE;
	}
}