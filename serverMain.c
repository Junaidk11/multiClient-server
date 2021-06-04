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
#define PORT 	8888

int main(int argc, char *argv[]){
	// Variable declarations
	int opt = TRUE;  		// Used for configuring Server socket to allow for multiple connections
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
	
	struct sockaddr_in address;     // Used for Socket Configurations
	
	char buffer[1025]; 	// A Buffer to store data sent by a client 

	fd_set readfds;  // A file descriptor for read operations
	char *message = "Acknowlegment message from the Server \r\n";

	// Initialize all client_socket[] to 0, i.e. server not connected to any socket
	for (i=0; i< max_clients; i++){
		client_sockets[i] = 0;
	}

	// Create a master socket, i.e. Server Socket  
	
	if ( (master_socket = socket(AF_INET, SOCK_STREAM, 0))==0){
		// Socket not created
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	// Master socket created, set socket configurations
	// The following is used to configure master socket
	// to allow for multiple client connections -> Best practices
	
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))<0){

	// Failed
	perror("setSockOpt");
	exit(EXIT_FAILURE);
	}	
	
	// Setting server socket configurations 
	address.sin_family = AF_INET;  		// The socket is assigned IPV4 Address
	address.sin_addr.s_addr = INADDR_ANY;  	// the socket can listen to listen connections over LAN, WIFI, or any other
	address.sin_port = htons(PORT);   	// The socket is assigned PORT number 8888, which is the localhost. Port can be any number between [1024, 6535]
	
	// Bind socket to server configurations set above
	
	if(bind(master_socket,(struct sockaddr *)&address, sizeof(address))<0){
		// Binding failed
		perror("Bind Failed");
		exit(EXIT_FAILURE);
	}

	// Listen for client connection requests
	
	printf("Listening on Port: %d \n", PORT);
	
	// Allow maximum of 2 client connection request pending
	if(listen(master_socket, 2)<0){
		perror("Listen");
		exit(EXIT_FAILURE);
	}

	
	// Accept incoming connection
	addrlen = sizeof(address);
	puts("Waiting for connections....");

	while(TRUE){

	}
	return 0;

}
