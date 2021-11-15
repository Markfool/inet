#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// #include <winsock2.h>

#define BUF_SIZE 1024

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[]){
    int serv_sock;
    int clnt_sock;
    char buf[BUF_SIZE];
    int read_cnt;
    FILE* fp;

    struct sockaddr_in serv_addr,clnt_addr;
    socklen_t clnt_adr_sz;

    if(argc != 2){
        printf("Usage : %s <port>\n",argv[1]);
        exit(1);
    }

    fp = fopen("file_serv.c","rb");
    if(fp == NULL){
        error_handling("fopen() error");
    }
    serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_UDP);
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
        error_handling("bind() error");
    }

    if(listen(serv_sock,5)==-1){
        error_handling("listen() error");
    }
    clnt_adr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr);
    if(clnt_sock == -1){
        error_handling("accept() error");
    }
    while(1){
        read_cnt = fread((void *)buf,1,BUF_SIZE,fp);
        if(read_cnt<BUF_SIZE){
            write(clnt_sock,buf,read_cnt);
            break;
        }
        write(clnt_sock,buf,BUF_SIZE);
    }
    shutdown(clnt_sock,SHUT_WR);
    read_cnt = read(clnt_sock,buf,BUF_SIZE);
    buf[read_cnt] = 0;
    printf("Message from client : %s \n",buf);
    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}