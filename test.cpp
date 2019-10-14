#include<stdio.h>
#include<string>
#include<cstring>
#include<malloc.h>
#include<math.h>
#include<iostream>

using namespace std;


int main(int argc, char const *argv[])
{
	// int sum = 5 ^ 7;
	// cout << sum;

char foo[] = "hello";
char test[]= "how are";

char full_text = (char[])malloc(strlen(foo)+strlen(test)+1); 
strcpy(full_text, foo ); 
strcat(full_text, test);

cout << full_text;
	return 0;
}

