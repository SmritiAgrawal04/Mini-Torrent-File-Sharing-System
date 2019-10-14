#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include<iostream>
#include <unistd.h> 
#include<vector>
#include <string.h> 
#include<cstring>
#include <stdlib.h>
#include<string>
#include <bits/stdc++.h> 
#include<fstream>
#include<unistd.h>
#include <netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<vector>
#include<algorithm>
#include <openssl/sha.h>

#define chunkSize 512*1024

using namespace std;
string user_id;

// unsigned int rotateleft(int x ,  int n)
// {
//     return (x << n) | (x >> (32-n));
// }

// unsigned int  SHA1(unsigned char* s)
// {
//     unsigned int h0,h1,h2,h3,h4;
//     h0 = 0x67452301;
//     h1 = 0xEFCDAB89;
//     h2 = 0x98BADCFE;
//     h3 = 0x10325476;
//     h4 = 0xC3D2E1F0;
//     int a,b,c,d,e,f,k,temp;

//     char* str;
//     str = (char *)malloc(strlen((const char *)s)+100);
//     strcpy((char *)str,(const char *)s);
//     unsigned int current_length = strlen((const char *)str);
//     unsigned int original_length = current_length;
//     str[current_length] = 0x80;
//     str[current_length + 1] = '\0';
//     char ic = str[current_length];
//     current_length++;
//     unsigned int ib = current_length % 64;

//     if(ib >= 56)
//         ib = 120 - ib;
//     else
//         ib = 56-ib;

//     for(int i=0;i < ib;i++)
//     {
//         str[current_length]=0x00;
//         current_length++;
//     }

//     str[current_length + 1]='\0';
//     int i;
//     for( i=0;i<6 ;i++)
//     {
//         str[current_length]=0x0;
//         current_length++;
//     }

//     str[current_length] = (original_length * 8) / 0x100 ;
//     current_length++;
//     str[current_length] = (original_length * 8) % 0x100;
//     current_length++;
//     str[current_length+i]='\0';
//     int number_of_chunks = current_length/64;
//     unsigned int word[80];
//     for(int i=0;i<number_of_chunks;i++)
//     {
//         for(int j=0;j<16;j++)
//         {
//             word[j] = str[i*64 + j*4 + 0] * 0x1000000 + str[i*64 + j*4 + 1] * 0x10000 + str[i*64 + j*4 + 2] * 0x100 + str[i*64 + j*4 + 3];
//         }
//         for( int j=16;j<80;j++)
//         {
//             int dummy = word[j-3] ^ word[j-8] ^ word[j-14] ^ word[j-16];
//             word[j] = rotateleft(dummy,1);
//         }   
//         a = h0;
//         b = h1;
//         c = h2;
//         d = h3;
//         e = h4;
//         for(int m=0;m<80;m++)
//         {
//             if(m<=19)
//             {
//                 f = (b & c) | ((~b) & d);
//                 k = 0x5A827999;
//             }
//             else if(m<=39)
//             {
//                 f = b ^ c ^ d;
//                 k = 0x6ED9EBA1;
//             }
//             else if(m<=59)
//             {
//                 f = (b & c) | (b & d) | (c & d);
//                 k = 0x8F1BBCDC;
//             }
//             else
//             {
//                 f = b ^ c ^ d;
//                 k = 0xCA62C1D6;
//             }
//             temp = (rotateleft(a,5) + f + e + k + word[m]) & 0xFFFFFFFF;
//             e = d;
//             d = c;
//             c = rotateleft(b,30);
//             b = a;
//             a = temp;
//         }
//         h0 = h0 + a;
//         h1 = h1 + b;
//         h2 = h2 + c;
//         h3 = h3 + d;
//         h4 = h4 + e;
//     }
    
//     // int h = h0+h1+h2+h3+h4;
//     // printf("%x\n", h0/0x10000);
//     printf("\n\n");
//     unsigned int h = h0*0x100000000;
//      // + h1)*0x10000000000000000 + h2/0x10000;

//     printf("%x\n" , h); 
//     // printf("%x%x%x\n", h0 , h1 , h2/0x10000);
//     // cout << "h0 = " << itoa(h0) << endl;
//     // cout << "h1 = " << to_string(h0) << endl;
//     // cout << "h2 = " << to_string(h0) << endl;
//     // cout << "h = " << h << endl;
//     printf("Hash: %x %x %x %x %x",h0, h1, h2, h3, h4);
//     printf("\n\n");
//     return h;
// }




int create_socket()
{
	return socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
}

int main(int argc, char const *argv[])
{
	/*int port;
	if(argc != 2)
		cout << "Invalid Port\n";
	else
		port = atoi(argv[1]);*/


	struct sockaddr_in add;
	add.sin_family = AF_INET;
	add.sin_port = htons(8080);


	int client_fd = create_socket();
	if(client_fd < 0)
	{
		perror("Socket Creation Failed!");
		exit(0);
	}

	// cout << client_fd << endl;

	if(inet_pton(AF_INET, "127.0.0.1", &add.sin_addr) <= 0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }

    int is_connect = connect(client_fd, (struct sockaddr *)&add, sizeof(add));

    if(is_connect < 0 )
    {
    	perror("Error Connecting!\n");
    	exit(0);
    }

    // cout << client_fd << endl;
    cout << "Connection with server successful!\n";

    FILE *f = fopen("/home/smriti/Downloads/Bills.pdf" , "rb");
    fseek(f , 0 , SEEK_END);
    int size = ftell(f) ;
    rewind(f);
    
    send(client_fd , &size , sizeof(size) , 0);

    unsigned char buffer[chunkSize];
    // unsigned char main_buffer[200000];
  	int n;
  	// string hash;
    while((n = fread(buffer , sizeof(char) , chunkSize , f)) > 0 && size > 0)
    {
    	unsigned char digest[SHA_DIGEST_LENGTH];
        char mdString[SHA_DIGEST_LENGTH*2];
        memset(buffer , '\0' , chunkSize);
    	SHA1((unsigned char*)buffer , strlen(buffer) , digest);
    	size = size-n;
    	int i = 0;
    	while(i < SHA_DIGEST_LENGTH)
    	{
    		sprintf((char*)&mdString[i*2], "%02x",digest[i]);
    		i++;
    	}
  		string hash_value(mdString);
  		string hash = hash_value.substr(0 ,20);
  		mdString[i] = '\0';
    }
    fclose(f);
    
    
	// cout << "userid = " << user_id;
	close(client_fd);
	return 0;
}