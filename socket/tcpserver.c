//
//  tcpserver.c
//  socket
//
//  Created by JackPan on 2021/12/12.
//
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#define MAX 8192

#define SA struct sockaddr
#define TRUE 1
// Function designed for chat between client and server.
void func(int sockfd, char *filepath)
{
    char buff[MAX];
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size;
    int line_count = 0;

    // infinite loop for chat
    bzero(buff, MAX);
    
    FILE *in;
    int count = 0;
    long int pos, old_pos, tmp_pos;
    in = fopen(filepath, "r");
    /* always check return of fopen */
    if (in == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fseek(in, 0, SEEK_END);
    pos = ftell(in);
    printf("pos is %ld \n", pos);
    old_pos = pos;
    /* Don't write each char on output.txt, just search for '\n' */
    while (pos) {
        fseek(in, --pos, SEEK_SET); /* seek from begin */
        if (fgetc(in) == '\n') {
            if (count++ == 10) {
                break;
            }
        }
    }
    /* Write line by line, is faster than fputc for each char */
    
    line_size = getline(&line_buf, &line_buf_size, in);
    while (line_size >= 0) {
        line_count++;
        printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", line_count, line_size, line_buf_size, line_buf);
        long write_number = write(sockfd, line_buf, line_size);
        printf("Write line buf number %ld\n", write_number);
        line_size = getline(&line_buf, &line_buf_size, in);
    }
//        free(line_buf);
    fclose(in);

    int monitor_num = 0;
    while (1) {
        in = fopen(filepath, "r");
        if (in == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        fseek(in, 0, SEEK_END);
        pos = ftell(in);
        printf("\n");
        printf("monitor_num: %d \n", monitor_num++);
        printf("current pos: %ld \n", pos);
        printf("old pos: %ld \n", old_pos);
        printf("connfd is %d\n", sockfd);

        tmp_pos = pos;
        int append_count = 0;
        while (pos > old_pos) {
            fseek(in, --pos, SEEK_SET); /* seek from begin */
            if (fgetc(in) == '\n') {
                append_count++;
            }
        }
        printf("append count is  %d \n", append_count);

        line_size = getline(&line_buf, &line_buf_size, in);
        while (line_size >= 0) {
            bzero(buff, MAX);
            line_count++;
            printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", line_count, line_size, line_buf_size, line_buf);
            long write_number = write(sockfd, line_buf, line_size);
            printf("Write line buf number %ld\n", write_number);
            printf("\n");
            line_size = getline(&line_buf, &line_buf_size, in);
        }
        size_t readNumber = read(sockfd, buff, MAX);
        if (readNumber == 0) {
            printf("read number is zero\n");
            break;
        }
        old_pos = tmp_pos;
        printf("Sleep 1 seconds %ld \n", old_pos);
        sleep(1);
//            free(line_buf);
        fclose(in);
        
    }

    printf("Socket is closed. \n");
        
}

// Driver function
int main(int agrc, char *agrv[])
{
    if (agrc < 2) {
        printf("You need add absoulate path in command");
        return 0;
    }
    
    
    char *filepath = agrv[1];
    printf("file path is %s\n", filepath);
    char *port_arr = agrv[2];
    printf("server port is %s\n", port_arr);
    int port = atoi(port_arr);
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
   
    // socket create and verification
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
   
    
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    // Now server is ready to listen and verification
    
   
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
   
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, (socklen_t*)&len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
    
    printf("connfd is %d\n", connfd);
   
    // Function for chatting between client and server
    func(connfd, filepath);
   
    // After chatting close the socket
    //close(sockfd);
}
