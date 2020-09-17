#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

const int PORT = 8080;

void initWinSock()
{
	printf("\nInitialising Winsock...");
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(1);
	}

	printf("Initialised\n");
}
struct Certificate
{
	unsigned int hash;
	char name[38];
};
void serialize(Certificate* packet, char* data);
int main() 
{ 
	int server_fd = 0;
	int new_socket = 0;
	struct sockaddr_in address; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	const char *hello = "Hello from server"; 
	
	initWinSock();

	// Create socket descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Attach socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	// Setup listen queue
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Waiting for connection...\n");
	//while (1)
	//{
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
			&addrlen)) < 0)
	{
			perror("accept");
			exit(EXIT_FAILURE);
	}
		//else {
			// start new thread ////....... 15 ms ...........
			// with new socket
			// if it hangs - need to inform with msg to clients instead of adding to buffer i.e. listen queue
		//}
	//}
	// send cert
	Certificate* serverstruct = new Certificate;
	serverstruct->hash = 2020;
	strcpy_s(serverstruct->name, "certificate authentification");
	char data[sizeof(Certificate)];
	serialize(serverstruct, data);
	send(new_socket, data, sizeof(data), 0);
	printf("\nCertificate sent!\n");
	printf("Waiting for data...\n");
	recv( new_socket , buffer, 1024, 0); 
	printf("Received: %s\n",buffer ); 
	send(new_socket , hello , strlen(hello) , 0 ); 
	printf("Hello message sent\n"); 
	return 0; 
} 
void serialize(Certificate* packet, char* data)
{
	int* q = (int*)data;
	*q = packet->hash;		q++;
	char* p = (char*)q;
	int i = 0;
	while (i < 38)
	{
		*p = packet->name[i];
		p++;
		i++;
	}
}