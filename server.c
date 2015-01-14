/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
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
     clilen = sizeof(cli_addr);
     
     while(1){
     //Step4:Accept a connection with the accept() system call. 
     //This call typically blocks until a client connects with the server.
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     
     //Step5 : Send and receive data
     n = read(newsockfd,buffer,255);
     if (n < 0){
     	error("ERROR reading from socket");
     } else if(strcmp(buffer,"Exit")){
     		close(newsockfd);
     		close(sockfd);
     }else{

		     printf("Here is the message: %s\n",buffer);
		     n = write(newsockfd,"I got your message",18);
		     if (n < 0) error("ERROR writing to socket");

     }

     }
     
     return 0; 
}
