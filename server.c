/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h> //for threading , link with lpthread

//the thread function
void *connection_handler(void *);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, client_sock, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     //Step1:Create a socket with the socket() system call
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     //Step2 :Bind the socket to a local address(ip) using the bind() system call. 
     //For a server socket on the Internet, an address consists of a port number on the host machine. 
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     //Step3:Listen for connections with the listen() system call
     // int listen(int socketfd,int backlog) backlog : the max connection num
     listen(sockfd,5);
     //Accept and incoming connection
     puts("Waiting for incoming connections...");
     clilen = sizeof(cli_addr);
     
     //Step4:Accept a connection with the accept() system call. 
     //This call typically blocks until a client connects with the server.
     
     // client_sock = accept(sockfd, 
     //             (struct sockaddr *) &cli_addr, 
     //             &clilen);
     // if (client_sock < 0) 
     //      error("ERROR on accept");
     bzero(buffer,256);
     
     pthread_t thread_id;
 	while( (client_sock = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen) ))
    {
        puts("Connection accepted");
         
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

     //Step5 : Send and receive data
     // n = read(client_sock,buffer,255);
     // if (n < 0) error("ERROR reading from socket");
     // printf("Here is the message: %s\n",buffer);
     // n = write(client_sock,"I got your message",18);
     // if (n < 0) error("ERROR writing to socket");

     // if(strcmp(buffer,"Exit")){
     // close(client_sock);
     // close(sockfd);
     // }
	
     

     return 0; 
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
     
    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , strlen(message));
     
    message = "Now type something and i shall repeat what you type \n";
    write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //end of string marker
		client_message[read_size] = '\0';
		
		//Send the message back to client
        write(sock , client_message , strlen(client_message));
		
		//clear the message buffer
		memset(client_message, 0, 2000);
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    return 0;
} 
