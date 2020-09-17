#include <stdio.h> 
#include <string.h> 

#include <WinSock2.h>
#include <WS2tcpip.h>

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
void deserialize(char* data, Certificate* packet);
int main() 
{ 
	int sock = 0; 
	struct sockaddr_in serv_addr; 
	const char *hello = "Hello from client"; 
	char buffer[1024] = {0}; 

	initWinSock();

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 

	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 
	//Connect to server
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	// recv cert
	Certificate* clientstruct = new Certificate;
	recv(sock, buffer, 1024, 0);
	deserialize(buffer, clientstruct);
	printf("Received certificate: name = %s \nhash = %d", clientstruct->name, clientstruct->hash);	
	printf("\nCertificate authenticated!\n");
	send(sock , hello , strlen(hello) , 0 ); 
	printf("Hello message sent\n"); 
	recv( sock , buffer, 1024, 0); 
	printf("Received: %s\n",buffer ); 
	return 0; 
} 
void deserialize(char* data, Certificate* packet)
{
	int* q = (int*)data;
	packet->hash = *q;		q++;
	char* p = (char*)q;
	int i = 0;
	while (i < 38)
	{
		packet->name[i] = *p;
		p++;
		i++;
	}
}