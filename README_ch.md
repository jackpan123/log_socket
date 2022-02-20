# log_socket

这是一个web日志监听的小工具，可以监听服务器端的日志，保存到本地。以供查看或者分析等作用。

##  快速入门

首先，需要先运行日志监听的服务端，通过上传tcpserver.c到对应的需要被监听的服务器上。运行以下命令：

```
gcc tcpserver.c -o tcpserver && ./tcpserver  /usr/local/business/online/nohup.out 8084
```

开头为编译该C代码，然后进行执行，接收两个参数：

1. /usr/local/business/online/nohup.out 日志文件所在的文件位置
2. 8084 服务器监听的端口号

当服务端启动完成后，会在8084端口进行监听，并且会不断监控日志文件的变化。

这时候就可以在本地电脑或者需要保存日志文件的服务器上，运行客户端程序，运行以下命令：

```
gcc tcpclient.c -o tcpclient && ./tcpclient /Users/jackpan/JackPanDocuments/temporary/c-test/edp.out 192.168.1.136 8084
```
参数详解：

1. /Users/jackpan/JackPanDocuments/temporary/c-test/edp.out 需要保存日志文件的文件位置
2. 192.168.1.136 目标主机的ip
3. 8084 服务器监听的端口号

启动完成后，日志会不断的采集到本地日志文件。



## 设计思路

这个小工具是采用socket实现的。

### 服务端

通过配置的参数，将服务端启动一个监听指定端口的socket，当客户端启动后，会建立起连接。服务器监听部分代码

```
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

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
```



通过getline函数去读取日志中的文件行数，如果监听到变化后，就会将日志行写入，客户端就会接收到对应的日志行进行保存。监听日志文件变化核心代码：

```
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
```



服务端检测客户端是否在线，如果不在线则退出监听，释放资源，代码如下：

```
        size_t readNumber = read(sockfd, buff, MAX);
        if (readNumber == 0) {
            printf("read number is zero\n");
            break;
        }
```



### 客户端

客户端接收到服务端的日志行后进行读取保存。代码如下：

```
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
```

