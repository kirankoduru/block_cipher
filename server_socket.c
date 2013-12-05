#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int main(int argc, char *argv[])
{
    int listen_file_descriptor = 0, connfd = 0;
    struct sockaddr_in serv_addr; //create a struct type sockaddr_in with name serv_addr

    char sendMessageBuffer[1025];
    time_t ticks; 

    // socket() params : AF_INET 		- namespace
	//    			     SOCK_STREAM 	- style : like a pipe, operates over a connection with a remote socket and transmits data as a stream of bytes
	//				     0 				- protocol for the kernel to decide default protocol (most times it is TCP)
	// socket() returns : int file descriptor
    listen_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendMessageBuffer, '0', sizeof(sendMessageBuffer)); 

    serv_addr.sin_family = AF_INET; // address family
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP address in network byte order 
    serv_addr.sin_port = htons(5000); // IP port

    // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
    // assigns an address specified by addr to the address to the socket reffered by the file descriptor sockfd
    bind(listen_file_descriptor, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listen_file_descriptor, 10); // max 10 connections for the listening socket

    while(1)
    {
        conn_file_descriptor = accept(listen_file_descriptor, (struct sockaddr*)NULL, NULL); 

        ticks = time(NULL);
        snprintf(sendMessageBuffer, sizeof(sendMessageBuffer), "%.24s\r\n", ctime(&ticks));
        write(conn_file_descriptor, sendMessageBuffer, strlen(sendMessageBuffer)); 

        close(conn_file_descriptor);
        sleep(1);
     }
}