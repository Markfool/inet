#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <winsock2.h>
#define BUF_SIZE 1024
void error_handling(char *message);
int main(int argc,char*argv[]){
    int serv_sock;
    int clnt_sock;
    char message[BUF_SIZE];

    struct sockaddr_in serv_addr,clnt_addr;
    if(argc!=2){
        printf("Usage : %s <port>",argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(serv_sock == -1){
        error_handling("socket() error");
    }
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1){
        error_handling("bind() error");
    }

    if(listen(serv_sock,5) == -1){
        error_handling("listen() error");
    }
    int size_clntaddr = sizeof(clnt_addr);
    for(int i=0;i<5;i++){
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&size_clntaddr);
        if(clnt_sock == -1){
            error_handling("accept() error");
        }
        else{
            printf("connecting counter: %d",i+1);
        }
        int str_len = 0;
        while((str_len = read(clnt_sock,message,BUF_SIZE))!=0){
            write(clnt_sock,message,str_len);
        }
        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
}


