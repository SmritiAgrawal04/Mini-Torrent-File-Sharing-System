#include <sys/socket.h>
#include<iostream>
#include <unistd.h> 
#include <stdio.h>  
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#define chunkSize 512000

using namespace std;
// char *ip = "127.0.0.1";

int create_socket()
{
	return socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
}

int bind_server(int server_fd , struct sockaddr_in address)
{
	return bind(server_fd , (struct sockaddr *)&address , sizeof(address));
}

int accept_request(int server_fd , struct sockaddr_in address , int addrlen)
{
	return accept(server_fd , (struct sockaddr *)&address , (socklen_t*)&addrlen); 
}

int main()
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);
	int addrlen = sizeof(address);
	int server_fd = create_socket();
	char command[100] = {0};

	if(!server_fd)
	{
		perror("Socket Creation Failed!");
		exit(0);
	}

	int is_bind = bind_server(server_fd , address);

	if(is_bind < 0)
	{
		perror("Binding Failed!");
		exit(0);
	}

	if(listen(server_fd , 3)  < 0)
	{
		perror("Error Listeining!");
		exit(0);
	}

	int new_socket = accept_request(server_fd , address , addrlen);

	if(new_socket < 0)
	{
		perror("Error Accepting!");
		exit(0);
	}

	FILE *f = fopen("/home/smriti/Desktop/output" , "wb");
   char buffer[chunkSize];
   int size , n;

   recv(new_socket , &size , sizeof(size) , 0);
   // cout<<"size "<<size;
   while (( n = recv( new_socket , buffer , chunkSize, 0) ) > 0  &&  size > 0)
   {
   
      fwrite (buffer , sizeof(char), n, f);
      memset ( buffer , '\0', chunkSize);
      size = size - n;
   }
	
	close(new_socket);
	close(server_fd);
	return 0;
}