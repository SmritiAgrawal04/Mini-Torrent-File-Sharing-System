#include <sys/socket.h>
#include<iostream>
#include <unistd.h> 
#include <stdio.h>  
#include <stdlib.h> 
#include <netinet/in.h>
#include<vector> 
#include<map>
#include <string.h>
#include<bits/stdc++.h> 
#include<pthread.h>
#define chunkSize 512000

using namespace std;

struct info
{
    int new_socket;
    int server_fd;
    int port;
};

map<string , pair<string , string>> database;
map<string , vector<int>> groups;
map<pair<string ,string> , pair<string , int>> files;
map<int , string> ports_info;
vector<int> current_users;


bool create_user(string user_id , string password , int new_socket , int server_fd)
{
    map<string , pair<string , string>>::iterator itr;
	bool present = false;
	char dummy[100] = {0};
	for(auto itr = database.find(user_id); itr != database.end(); itr++)
		present = true;

	if(present)
	{
		string temp = "This user id already exists.\n";
    	strcpy(dummy , temp.c_str());
    	send(new_socket , dummy , strlen(dummy) , 0);
		return false;
	}

	else
	{
		pair<string , pair<string , string>> p;
		p.first = user_id;
		p.second.first = password;
		p.second.second = "-1";
		database.insert(p);
	}
    return true;
}

bool cmpstr(string temp , string password)
{
    // cout <<"Stringtemp = " << temp << " " << "string password = " << password << endl;
    if(temp.length() != password.length())
        return false;

    else
    {
        for(int i = 0 ; i < temp.length() ; i++)
        {
            if(temp[i] != password[i])
                return false;
        }
    }
    return true;
}

void login(string user_id , string password , int new_socket , int server_fd)
{
	// bool present = false;
    char dummy[100] = {0};
    string temp = "login";
    strcpy(dummy , temp.c_str());
    send(new_socket , dummy , strlen(dummy) , 0);
    int port;
    recv(new_socket , &port , sizeof(port) , 0);
    map<string , pair<string , string>>::iterator itr;
    itr = database.find(user_id);

    if(itr != database.end())
    {
        string temp = itr->second.first;
        // cout << "in login temp = "<< temp << endl;

        if(cmpstr(temp , password ))
        {
            // cout << "inside if\n";
            // logged_in = true;
            string temp = "Login Successful.\n";
            strcpy(dummy , temp.c_str());
            send(new_socket , dummy , strlen(dummy) , 0);
            pair<int  , string> p;
            // cout << farzi;
            p.first = port;
            p.second = user_id;
            ports_info.insert(p);
            current_users.push_back(port);
            // cout << "port info added\n";
            cout <<"size = " << current_users.size();
            for(int i = 0 ; i < current_users.size() ; i++)
                cout << current_users[i];

        }
    }

    else
    {
        string temp = "Invalid User Id or Password.\n";
        strcpy(dummy , temp.c_str());
        send(new_socket , dummy , strlen(dummy) , 0);
    }
}

bool check_active(int owner)
{
    vector<int>::iterator itr;
    itr = find(current_users.begin() , current_users.end() , owner);
    if(itr != current_users.end())
        return true;
    else
        return false;

}

void create_group(string group_id , int new_socket)
{
    char dummy[100] = {0};
    string temp = "create_group";
    strcpy(dummy , temp.c_str());
    send(new_socket , dummy , strlen(dummy) , 0);
    int owner;
    recv(new_socket , &owner , sizeof(owner) , 0);

    bool present = check_active(owner);
    // cout << "after recv\n";
    // string owner = string(dummy);
    if(present)
    {
        map<string , vector<int>>:: iterator itr;
        itr = groups.find(group_id);
        if(itr == groups.end())
        {
            map<string , string>:: iterator itr;
            pair<string , int> p;
            p.first = group_id;
            p.second = owner;
            groups.insert(p);
            string temp = "Group created Successfully.\n";
            strcpy(dummy , temp.c_str());
            send(new_socket , dummy , strlen(dummy) , 0);
        }
        
        else
        {
            char dummy[100] = {0};
            string temp = "This Group Id already exists.\n";
            strcpy(dummy , temp.c_str());
            send(new_socket , dummy , strlen(dummy) , 0);
        }
    
    }
    else
    {
        char dummy[100] = {0};
        string temp = "Please Login.\n";
        strcpy(dummy , temp.c_str());
        send(new_socket , dummy , strlen(dummy) , 0);   
    }
    
}

void join_group(string group_id , int new_socket)
{   
    /*map<int , string>:: iterator itr;
    itr = groups.find(group_id);

    if(itr == groups.end())
    {
        char dummy[100] = {0};
        string temp = "This Group Id does not exist.\n";
        strcpy(dummy , temp.c_str());
        send(new_socket , dummy , strlen(dummy) , 0);
    }

    else
    {
        int owner = itr->second;

    }*/

}

void leave_group(string group_id , int new_socket)
{
    map<string , vector<int>>:: iterator itr;
    itr = groups.find(group_id);

    if(itr == groups.end())
    {
        char dummy[100] = {0};
        string temp = "Invalid Group Id.\n";
        strcpy(dummy , temp.c_str());
        send(new_socket , dummy , strlen(dummy) , 0);
    }

    else
    {
        //check if the user is a member of group or not?
        //set grpid = -1 corresponding to the userid of requested client if true
        bool present =false;
        char dummy[100] = {0};
        string temp = "leave_group";
        strcpy(dummy , temp.c_str());
        send(new_socket , dummy , strlen(dummy) , 0);
        int owner , i;
        recv(new_socket , &owner , sizeof(owner) , 0);

        for( i = 0 ; i < (itr->second).size() ; i++)
        {
            if((itr->second)[i] == owner)
            {
                present = true;
                break;
            }
        }
        if(present)
        {
            (itr->second).erase((itr->second).begin() + i);
            char dummy[100] = {0};
            string temp = "Removed from Group.\n";
            strcpy(dummy , temp.c_str());

        }
        else
        {
            char dummy[100] = {0};
            string temp = "The client is not a member of the group.\n";
            strcpy(dummy , temp.c_str());
        }

    }
}

void list_requests(string group_id)
{

}

void accept_requests(string group_id , string user_id)
{

}

void list_groups()
{
    map<string , vector<int>>:: iterator itr;
    for(itr = groups.begin() ; itr != groups.end() ; ++itr)
        cout << itr->first << endl;
}

void list_files(string group_id)
{

}

void upload_files(string file_path , string group_id , int new_socket)
{
    char dummy[100] = {0};
    int file_size ;
    string temp = "get_sha\n";
    strcpy(dummy , temp.c_str());
    send(new_socket , dummy , strlen(dummy) , 0);
    memset(dummy ,'0' , 100);
    strcpy(dummy , file_path.c_str());
    send(new_socket , dummy , strlen(dummy) ,0);
    memset(dummy ,'0' , 100);

    // int file_size;
    recv(new_socket , &file_size , sizeof(int) , 0);
    // file_size = atoi(dummy);
    // cout << "sfter recieving size\n" << file_size << endl;
    cout << endl;
    // cout << "sfter recieving size\n" << file_size << endl;
    int partitions = ceil(file_size/(512*1024)) + 1;
    cout << "file_size = " << file_size << endl;
    cout << "partitions = " << partitions <<endl;
    int i = 0;
    while(i < partitions){
        char sha[20];
        recv(new_socket , sha , strlen(sha) , 0);
        cout << endl;
        files.insert(make_pair(make_pair(string(sha) , group_id) , make_pair(file_path , file_size)));
        i++;
    }
    
}

void download_file(string group_id , string file_name , string destination_path)
{

}

void logout(int new_socket)
{
    char dummy[100] = {0};
    string temp = "logout";
    strcpy(dummy , temp.c_str());
    send(new_socket , dummy , strlen(dummy) , 0);
    int owner;
    recv(new_socket , &owner , sizeof(owner) , 0);

    // vector<int>::iterator itr;
    int index = find(current_users.begin() , current_users.end() , owner) - current_users.begin();
    current_users.erase(current_users.begin() + index);
    memset(dummy , '\0' , 100);
    temp = "Logout Successful!!\n";
    strcpy(dummy , temp.c_str());
    send(new_socket , dummy , strlen(dummy) , 0);


}

void show_downloads()
{

}

void stop_share(string group_id , string file_name)
{

}

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

void* check_input(void* sock)
{
	
	// cout << "inside while\n";
	char command[100] = {0};
	char dummy[100] = {0};	
    struct info *data;
    data = (struct info *) sock;
	int valread = read(data->new_socket , command , 100);

	vector<char*> arguments;
    char* temp = strtok(command , " ");
    while(temp != NULL)
    {
    	arguments.push_back(temp);
    	temp = strtok(NULL," ");
    }
    
    if(strcmp(arguments[0] , "create_user") == 0)
    {
    	// cout << "inisde craete user if\n";
    	if(arguments.size() == 3)
        {
    		bool done = create_user(arguments[1] , arguments[2] , data->new_socket , data->server_fd);
    		// for(itr = database.begin(); itr != database.end(); ++itr)
    		// 	cout << itr->first << '\t' << itr->second.first << '\t' << itr->second.second <<  endl;
            if(done)
            {
                string temp = "User Created Successfully\n";
                strcpy(dummy , temp.c_str());
                send(data->new_socket , dummy , strlen(dummy) , 0);
            }
    	}

    	else
    	{
    		string temp = "Invalid Arguments!!\n";
    		strcpy(dummy , temp.c_str());
    		send(data->new_socket , dummy , strlen(dummy) , 0);
    	}
    }

    else if(strcmp(arguments[0] , "login") == 0)
    {
    	if(arguments.size() == 3){
    		login(arguments[1] , arguments[2] , data->new_socket , data->server_fd);
            // map<int , string>:: iterator itr;
            // for(itr = ports_info.begin() ; itr != ports_info.end() ; ++itr)
            //     cout << itr->first << '\t' << itr->second << endl;


    	}

    	else
    	{
    		string temp = "Invalid Arguments!!\n";
    		strcpy(dummy , temp.c_str());
    		send(data->new_socket , dummy , strlen(dummy) , 0);
    	}
    }

    else if(strcmp(arguments[0] , "create_group") == 0)
    {
    	if(arguments.size() == 2)
        {
    		  create_group(arguments[1] , data->new_socket);
            // map<string , int>:: iterator itr;
            // for(itr = groups.begin(); itr != groups.end(); ++itr)
            //     cout << itr->first << '\t' << itr->second << endl;
        }

    	else
    	{
    		string temp = "Invalid Arguments!!\n";
    		strcpy(dummy , temp.c_str());
    		send(data->new_socket , dummy , strlen(dummy) , 0);
    	}
    }

    else if(strcmp(arguments[0] , "join_group") == 0)
    {
    	if(arguments.size() == 2)
    		join_group(arguments[1] , data->new_socket);
    	else
    	{
    		string temp = "Invalid Arguments!!\n";
    		strcpy(dummy , temp.c_str());
    		send(data->new_socket , dummy , strlen(dummy) , 0);
    	}
    }

    else if(strcmp(arguments[0] , "leave_group") == 0)
    {
    	if(arguments.size() == 2)
    		leave_group(arguments[1] , data->new_socket);
    	else
    	{
    		string temp = "Invalid Arguments!!\n";
    		strcpy(dummy , temp.c_str());
    		send(data->new_socket , dummy , strlen(dummy) , 0);
    	}
    }

    else if(strcmp(arguments[0] , "list_requests") == 0)
    {
    	if(arguments.size() == 2)
    		list_requests(arguments[1]);
    	else
    	{
    		string temp = "Invalid Arguments!!\n";
    		strcpy(dummy , temp.c_str());
    		send(data->new_socket , dummy , strlen(dummy) , 0);
    	}
    }

    else if(strcmp(arguments[0] , "accept_requests") == 0)
    {
    	if(arguments.size() == 3)
    		accept_requests(arguments[1] , arguments[2]);
    	else
    	{
    		string temp = "Invalid Arguments!!\n";
    		strcpy(dummy , temp.c_str());
    		send(data->new_socket , dummy , strlen(dummy) , 0);
    	}
    }

    else if(strcmp(arguments[0] , "list_groups") == 0)
    		list_groups();
    
    else if(strcmp(arguments[0] , "list_files") == 0)
    {
    	if(arguments.size() == 2)
    		list_files(arguments[1]);
    	else
    	{
    		string temp = "Invalid Arguments!!\n";
    		strcpy(dummy , temp.c_str());
    		send(data->new_socket , dummy , strlen(dummy) , 0);
    	}
    }

    else if(strcmp(arguments[0] , "upload_files") == 0)
    {
    	if(arguments.size() == 3){
    		upload_files(arguments[1] , arguments[2] , data->new_socket);
            map<pair<string ,string> , pair<string , int>>::iterator itr;
            for(itr = files.begin(); itr != files.end(); ++itr)
                cout << itr->first.first << '\t' << itr->first.second << '\t' << itr->second.first << '\t' << itr->second.second << endl;
        }

    	else
    	{
    		string temp = "Invalid Arguments!!\n";
    		strcpy(dummy , temp.c_str());
    		send(data->new_socket , dummy , strlen(dummy) , 0);
    	}
    }

    else if(strcmp(arguments[0] , "download_file") == 0)
    {
    	if(arguments.size() == 4)
    		download_file(arguments[1] , arguments[2] , arguments[3]);
    	else
    	{
    		string temp = "Invalid Arguments!!\n";
    		strcpy(dummy , temp.c_str());
    		send(data->new_socket , dummy , strlen(dummy) , 0);
    	}
    }

    else if(strcmp(arguments[0] , "logout") == 0){
    		logout(data->new_socket);
            // vector<int>::iterator itr;
            for(int i = 0 ; i < current_users.size() ; i++)
                cout << current_users[i];
        }
    	
    else if(strcmp(arguments[0] , "show_downloads") == 0)
    		show_downloads();

    else if(strcmp(arguments[0] , "stop_share") == 0)
    {
    	if(arguments.size() == 3)
    		stop_share(arguments[1] , arguments[2]);
    	else
    	{
    		string temp = "Invalid Arguments!!\n";
    		strcpy(dummy , temp.c_str());
    		send(data->new_socket , dummy , strlen(dummy) , 0);
    	}
    }

    else
    {
    	string temp = "Invalid Command!!\n";
    	strcpy(dummy , temp.c_str());
    	send(data->new_socket , dummy , strlen(dummy) , 0);
    }

    /*string fokat = "Task Accomplished!";
    strcpy(dummy , fokat.c_str());
    send(data->new_socket , dummy , strlen(dummy) , 0);*/
    check_input(sock);	
}



int main()
{
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int port_num;
	int server_fd = create_socket();
	port_num = 8081;
	pthread_t thread[2];
    struct info data[2];

	if(!server_fd)
	{
		perror("Socket Creation Failed!");
		exit(0);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port_num);

	int is_bind = bind_server(server_fd , address);

	if(is_bind < 0)
	{
		perror("Binding Failed!");
		exit(0);
	}

	if(listen(server_fd , 2)  < 0)
	{
		perror("Error Listening!");
		exit(0);
	}

	int i = 0;
	int new_socket;
	while(i < 2)
	{
		new_socket = accept_request(server_fd , address , addrlen);
		// cout << "in main = " << ntohs(address.sin_port) << endl;
        int port = ntohs(address.sin_port);
		if(new_socket < 0)
		{
			perror("Error Accepting!");
			exit(0);
		}
        
        data[i].new_socket = new_socket;
        data[i].server_fd = server_fd;
        data[i].port = port;
		pthread_create(&thread[i] , NULL , check_input , (void*)&data[i]);
		i++;
	}

	for(int i = 0 ; i < 2 ; i++)
	{
		pthread_join(thread[i] , NULL);
	}

	// check_input(new_socket);

	close(new_socket);
	close(server_fd);
	return 0;
}