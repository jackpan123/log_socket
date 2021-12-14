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
#define MAX 512
#define PORT 8080
#define SA struct sockaddr
#define TRUE 1
// Function designed for chat between client and server.
void func(int sockfd, char *filepath)
{
    char buff[MAX];
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);
   
        // read the message from client and copy it in buffer
//        read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents
//        printf("From client: %s\t To client : ", buff);
//        bzero(buff, MAX);
//        n = 0;
        // copy server message in the buffer
//        while ((buff[n++] = getchar()) != '\n')
//            ;
   
        // and send that buffer to client

//        strcpy(buff, "This is some text");
        
        FILE *in;
        int count = 0;
        long int pos, old_pos, tmp_pos;
        char s[512];

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
        while (fgets(s, sizeof(s), in) != NULL) {
            write(sockfd, s, sizeof(s));
        }
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
            tmp_pos = pos;
            int append_count = 0;
            while (pos > old_pos) {
                fseek(in, --pos, SEEK_SET); /* seek from begin */
                if (fgetc(in) == '\n') {
                    append_count++;
                }
            }
            printf("append count is  %d \n", append_count);

            /* Write line by line, is faster than fputc for each char */
            while (fgets(s, sizeof(s), in) != NULL) {
                printf("%s \n", s);
                write(sockfd, s, sizeof(s));
            }
            old_pos = tmp_pos;
            printf("Sleep 1 seconds %ld \n", old_pos);
            sleep(1);
            fclose(in);
        }
        
   
        // if msg contains "Exit" then server exit and chat ended.
//        if (strncmp("exit", buff, 4) == 0) {
//            printf("Server Exit...\n");
//            break;
//        }
    }
}

// Driver function
int main(int agrc, char *agrv[])
{
    if (agrc < 2) {
        printf("You need add absoulate path in command");
        return 0;
    }
    
    printf("%s", agrv[1]);
    char *filepath = agrv[1];
    printf("%s", filepath);
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
    servaddr.sin_port = htons(PORT);
   
    
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
    printf("hahah\n");
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
   
    // Function for chatting between client and server
    func(connfd, filepath);
   
    // After chatting close the socket
    //close(sockfd);
}
