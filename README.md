English | [简体中文](README_ch.md)

# log_socket

##  Getting Started
Run tcp server, you should in your remote machine
```
gcc tcpserver.c -o tcpserver && ./tcpserver  /usr/local/business/online/nohup.out 8084
```

When you run server completely, you can run tcp client. 
```
gcc tcpclient.c -o tcpclient && ./tcpclient /Users/jackpan/JackPanDocuments/temporary/c-test/edp.out 192.168.88.136 8084
```
And enjoy you remote tail comamnd. You can find log record in your file path (/Users/jackpan/JackPanDocuments/temporary/c-test/edp.out)
