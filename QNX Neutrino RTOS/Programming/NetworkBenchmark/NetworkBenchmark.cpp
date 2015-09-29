#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) 
{	
	const unsigned short PORT = 1234;
	const unsigned int BUFFER_SIZE = 2048;	
	
	int serverSocket, clientSocket;
	int size = 0;
	char* buffer = new char[BUFFER_SIZE];
	struct sockaddr_in server;
	
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket < 0)
	{
		printf("Error\n");
		exit(1);
	}
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	if(bind(serverSocket, (struct sockaddr*)&server, sizeof(sockaddr)))	
	{
		printf("Error\n");
		exit(1);
	}
	listen(serverSocket, 5);
	while(1)
	{
		printf("Listening at Port %d...\n", PORT);
		clientSocket = accept(serverSocket, 0, 0);		
		if(clientSocket < 0)
		{
			printf("Error\n");
			exit(1);
		}
		printf("Benchmarking...\n");
		do
		{
			size = read(clientSocket, buffer, BUFFER_SIZE);			
			//write(clientSocket, buffer, size);
		} while(size > 0);				
		close(clientSocket);
	}		
	close(serverSocket);
	printf("OK\n");
	delete buffer;

	return EXIT_SUCCESS;	
}
