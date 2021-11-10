#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// #include <winsock2.h>
#define BUF_SIZE 1024
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
}

int main(int argc,char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];
    int str_len;

    if(argc!=3){
        printf("Usage : %s <ip> <port>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    // sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1){
        error_handling("connect() error");
    }
    else
        printf("connect successfully");
    
    while (1)
    {
        fputs("input message(Q to exit):",stdout);
        fgets(buf,BUF_SIZE,stdin);
        if(!strcmp(buf,"q\n")||!strcmp(buf,"Q\n"))
            break;
        write(sock,buf,strlen(buf));
        str_len = read(sock,buf,BUF_SIZE-1);
        buf[str_len] = 0;
        printf("Message from server : %s",buf);
    }
    close(sock);
    return 0;
    
}
