//
//  tcpclient.c
//  socket
//
//  Created by JackPan on 2021/12/12.
//

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <wchar.h>
#define MAX 1024
#define SA struct sockaddr
void func(int sockfd, char *filepath)
{
    FILE *fp;
    char buff[MAX];
    for (;;) {
        bzero(buff, MAX);
        read(sockfd, buff, MAX);
        printf("%s", buff);
        fp = fopen(filepath, "a+");
        fprintf(fp, "%s", buff);
        fclose(fp);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}
   
int main(int agrc, char* agrv[])
{
    printf("%s", agrv[1]);
    char *filepath = agrv[1];
    printf("%s", filepath);
    char *port_arr = agrv[2];
    int port = atoi(port_arr);
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.38.16");
    servaddr.sin_port = htons(port);
   
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
   
    // function for chat
    func(sockfd, filepath);
   
    // close the socket
    close(sockfd);
}
