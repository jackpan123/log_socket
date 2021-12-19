# log_socket
##  Getting Started
Run tcp server, you should in your remote machine
```
gcc tcpserver.c -o tcpserver && ./tcpserver /usr/local/spring-edp/edp.out
```

When you run server completely, you can run tcp client. 
```
gcc tcpclient.c -o tcpcliennt && ./tcpclient /Users/jackpan/local/analyze/edp.out 8085 192.168.1.102
```
And enjoy you remote tail comamnd.
