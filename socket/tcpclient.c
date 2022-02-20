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
#define MAX 8192
#define SA struct sockaddr
void func(int sockfd, char *filepath)
{
    FILE *fp;
    char buff[MAX];
    for (;;) {
        bzero(buff, MAX);
        size_t readNumber = read(sockfd, buff, MAX);
        if (readNumber == 0) {
            printf("read number is zero\n");
            break;
        }
        printf("%s", buff);
        long write_number = write(sockfd, buff, MAX);
        printf("Write line buf number %ld\n", write_number);
        fp = fopen(filepath, "a+");
        fprintf(fp, "%s", buff);
        fclose(fp);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
    
    printf("Socket is closed.");
}
   
int main(int agrc, char* agrv[])
{
    char *filepath = agrv[1];
    printf("file path is %s\n", filepath);
    char *monitor_ip = agrv[2];
    printf("server ip is %s\n", monitor_ip);
    char *port_arr = agrv[3];
    printf("server port is %s\n", port_arr);
    int port = atoi(port_arr);
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(monitor_ip);
    servaddr.sin_port = htons(port);
   
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
   
    func(sockfd, filepath);
   
    close(sockfd);
}
