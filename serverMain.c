 /*
  *  Using Select() to setup a multiClient-Server Communication
  */

// Header Files need for socket programming on Linux
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

// Macros
#define TRUE 	1
#define FALSE 	0
#define PORT 	888

int main(int argc, char *argv[]){
	// Variable declarations
	int master_socket; 		
        int addrlen;
        int client_soc; 
        int client_sockets[10];
        int max_clients=10;
       	int activity;
        int i;
	int valread;
        int sd; 
	int max_sd; 			// Maximum file descriptors
	
	struct sockaddr_in address; 
	
	return 0;

}
