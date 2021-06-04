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

		// After select() used, ALWAYS reset the descripter set for each operation
		// clear socket set
		FD_ZERO(&readfds);

		// add master to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket; 

		// Print Server socket value
		printf("Value of master socket is: %d \n", master_socket);
		

		// Add child sockets to the set
		
		for(i=0; i<max_clients; i++){

			printf("Adding child socket to the set. \n");

			sd = client_sockets[i];

			if(sd>0){
				FD_SET(sd, &readfds);
			}

			// Highest file descriptor number -> need for select() 
			if(sd > max_sd){
				max_sd = sd;
			}
		} // End of for-loop


		printf("BEFORE SELECT \n");

		// Wait for an activity on one of the sockets 
		// Timeout is set to NULL, i.e. wait until a client socket is ready for a read operation
		activity = select(max_sd+1, &readfds, NULL, NULL, NULL);
		printf("AFTER SELECT \n");

		// Check for any kind of activity error
		if( (activity < 0) && (errno!=EINTR)){
			printf("Select() error");
		}
		
		// There was activity on the server socket 
		// i.e. A client wants to establish connection with the server
		if(FD_ISSET(master_socket, &readfds)){
			
			if((client_soc = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
				// Failed to establish connection with the client
				perror("Accept error");
				exit(EXIT_FAILURE);
			}

			// New connection established.
			// Inform user of socket number assigned to the new client, this socket number used in send/receive commands
			printf("New connection, Socket fd: %d, IP: %s, Port: %d \n", client_soc, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
			// Send New Connection acknowledgment message to the client 
			if(send(client_soc, message, strlen(message), 0) != strlen(message)){
				// Sending message to client failed. 
				perror("Server ack failed");
			}

			puts("Acknowledgment message sent successfully by the Server to the client.");

			// Add the new client connection to the set

			for(i=0; i<max_clients; i++){

				// Add new client to the set if there is room 
				if(client_sockets[i] ==0){
					client_sockets[i] = client_soc;
					break;
				}
			}
			
		}
		 
		// The activity  is some I/O operation on some other socket in the set 
		
		for (i=0; i<max_clients; i++){
			
			sd = client_sockets[i];

			printf("I//O Operation on Client socket: %d \n", sd);

			if(FD_ISSET(sd, &readfds)){
				// If a  Send/Recieve was set on the respective client socket
				// That client socket is now ready for a read operation
				
				printf("Client Socket for reading is set. \n");

				// Check if client socket sent messages, else close the connection
				if((valread = read(sd, buffer, 1024))==0){

					// No bytes recieved from the client
					printf("No bytes retured from read. \n");

					// Close the respective socket and mark as 0 in the client socket list
					close(sd);
					client_sockets[i] = 0;
				}else{
				 	// Read bytes from the client socket,
					// echo the message read from the client back to the client
					
					printf("Client Message: %s \n", buffer);

					buffer[valread] = '\0';

					send(sd, buffer, strlen(buffer), 0); // Sending a message to the client socket
				}
			}
		} // End for-loop

	} // End while-loop

	return 0;

}
