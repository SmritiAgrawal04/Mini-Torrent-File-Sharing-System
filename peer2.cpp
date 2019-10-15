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
int port = 8083;

/*unsigned int rotateleft(unsigned int x , unsigned int n)
{
    return (x << n) | (x >> (32-n));
}

void SHA1(unsigned char* s)
{
    unsigned  int h0,h1,h2,h3,h4;
    h0 = 0x67452301;
    h1 = 0xEFCDAB89;
    h2 = 0x98BADCFE;
    h3 = 0x10325476;
    h4 = 0xC3D2E1F0;
    unsigned int a,b,c,d,e,f,k,temp;

    unsigned char* str;
    str = (unsigned char *)malloc(strlen((const char *)s)100);
    strcpy((char *)str,(const char *)s);
    int current_length = strlen((const char *)str);
    int original_length = current_length;
    str[current_length] = 0x80;
    str[current_length + 1] = '\0';
    char ic = str[current_length];
    current_length++;
    int ib = current_length % 64;

    if(ib >= 56)
        ib = 120 - ib;
    else
        ib = 56-ib;

    for(int i=0;i < ib;i++)
    {
        str[current_length]=0x00;
        current_length++;
    }

    str[current_length + 1]='\0';
    int i;
    for( i=0;i<6;i++)
    {
        str[current_length]=0x0;
        current_length++;
    }

    str[current_length] = (original_length * 8) / 0x100 ;
    current_length++;
    str[current_length] = (original_length * 8) % 0x100;
    current_length++;
    str[current_length+i]='\0';
    int number_of_chunks = current_length/64;
    unsigned int word[80];
    for(int i=0;i<number_of_chunks;i++)
    {
        for(int j=0;j<16;j++)
        {
            word[j] = str[i*64 + j*4 + 0] * 0x1000000 + str[i*64 + j*4 + 1] * 0x10000 + str[i*64 + j*4 + 2] * 0x100 + str[i*64 + j*4 + 3];
        }
        for( int j=16;j<80;j++)
        {
            unsigned int dummy = word[j-3] ^ word[j-8] ^ word[j-14] ^ word[j-16];
            word[j] = rotateleft(dummy,1);
        }   
        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;
        for(int m=0;m<80;m++)
        {
            if(m<=19)
            {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if(m<=39)
            {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if(m<=59)
            {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else
            {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            temp = (rotateleft(a,5) + f + e + k + word[m]) & 0xFFFFFFFF;
            e = d;
            d = c;
            c = rotateleft(b,30);
            b = a;
            a = temp;
        }
        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
    }
    

    // printf("%x\n", h0/0x10000);
    printf("\n\n");
    printf("%x%x%x\n", h0 , h1 , h2/0x10000);
    // printf("Hash: %x %x %x %x %x",h0, h1, h2, h3, h4);
    printf("\n\n");
}
*/

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

void *Server(void *threadid)
{
   struct sockaddr_in address;
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(port);
   int addrlen = sizeof(address);
   int server_fd = create_socket();

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
      perror("Error Acc epting!");
      exit(0);
   }

   // FILE *f = fopen("/home/smriti/Desktop/output" , "wb");
   // char buffer[chunkSize];
   int size , n;

   recv(new_socket , &size , sizeof(size) , 0);
   // cout<<"size "<<size;
   /*while (( n = recv( new_socket , buffer , chunkSize, 0) ) > 0  &&  size > 0)
   {
   
      fwrite (buffer , sizeof(char), n, f);
      memset ( buffer , '\0', chunkSize);
      size = size - n;
   }*/

   // send(new_socket , hello ,  strlen(hello) , 0);
   // cout << "\nMessage sent to client." << endl;
   close(new_socket);
   close(server_fd);
   pthread_exit(NULL);
}

void quit()
{
  exit(0);
}

void *Client(void *threadid) 
{

   // int x ;
   // cin >> x;
   struct sockaddr_in add;
   add.sin_family = AF_INET;
   add.sin_port = htons(8081);


   // char *hello = "A hello from client.";

   int client_fd = create_socket();
   if(client_fd < 0)
   {
      perror("Socket Creation Failed!");
      exit(0);
   }

   if(inet_pton(AF_INET, "127.0.0.1", &add.sin_addr) <= 0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        exit(-1); 
    }

    int is_connect = connect(client_fd, (struct sockaddr *)&add, sizeof(add));

    if(is_connect < 0 )
    {
      perror("Eror Connecting!\n");
      exit(0);
    }

    cout << "Connection with server successful!\n";

    
    while(1){
    // cout << "Enter your command:\n1. Create User\n2. Login\n3. Create Group\n4. Join Group\n5. Leave Group\n6. List Requests\n7. Accept Requests\n8. List Groups\n9. List Files\n10. Upload File\n11. Download File\n12. Logout\n13. Show Downloads\n14. Stop Sharing\n";
      char command[100] = {0};
      char dummy[100] = {0};
      cin.getline(command , 100);

      send(client_fd , command , strlen(command) , 0);

      int valread = read(client_fd , dummy , 100);
      // cout <<"before comp = " << dummy << endl;
      string temp = string(dummy);
      
      if(temp == "login")
      {
        // cout << "in peer before sending\n";
        send(client_fd , &port , sizeof(port) , 0);
        // cout << "in peer before sending\n";
        int valread = read(client_fd , dummy , 100);
        cout << dummy;
      }

      else if(temp == "create_group")
      {
        // cout << "in peer before sending\n";
        send(client_fd , &port , sizeof(port) , 0);
        // cout << "in peer before sending\n";
        int valread = read(client_fd , dummy , 100);
        cout << dummy;
      }

      else if(temp == "logout")
      {
        // cout << "in peer before sending\n";
        send(client_fd , &port , sizeof(port) , 0);
        // cout << "in peer before sending\n";
        int valread = read(client_fd , dummy , 100);
        cout << dummy;
        quit();
      }

      else if(temp == "leave_group")
      {
        // cout << "in peer before sending\n";
        send(client_fd , &port , sizeof(port) , 0);
        // cout << "in peer before sending\n";
        int valread = read(client_fd , dummy , 100);
        cout << dummy;
      }

      else if(temp == "get_sha")
      {
        // cout << "in peer before sending\n";
        cout << "in sha\n";
        char dummy[200] = {0};
        recv(client_fd , dummy , 100 , 0);
        cout << "after read\n";

        char* path = dummy;
        cout << "path = " <<path <<endl;
        memset ( dummy , '\0', 200);

        FILE *f = fopen(path , "rb");
        fseek(f , 0 , SEEK_END);
        int size = ftell(f) ;
        rewind(f);
        cout << "file_size = " << size << endl;
        send(client_fd , &size , sizeof(size) , 0);
        cout << "after sending size\n";

        /*char buffer[chunkSize];
        // unsigned char main_buffer[200000];
        int n;
        
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
          char sha[20] = {0};
          strcpy(sha , hash.c_str());
          send(client_fd , sha , strlen(sha) , 0);
          mdString[i] = '\0';
        }*/
        fclose(f);
        // send(client_fd , &port , sizeof(port) , 0);
        // // cout << "in peer before sending\n";
        // valread = read(client_fd , dummy , 100);
        // cout << dummy;
      }



      else
        cout << dummy << endl;
      
  }
  // cout << "userid = " << user_id;
   close(client_fd);
   // pthread_exit(NULL);
}

int main () 
{
   pthread_t thread1 , thread2;
   int r1,r2;
   
   r1 = pthread_create(&thread1, NULL, Server , (void *)0);
      
   if (r1) 
   {
      cout << "Error:unable to create thread1," << r1 << endl;
      exit(-1);
   }

   
   r2 = pthread_create(&thread2, NULL , Client , (void *)1);

   if (r2) 
   {
      cout << "Error:unable to create thread2," << r2 << endl;
      exit(-1);
   }

   pthread_exit(NULL);
   return 0;
 }
