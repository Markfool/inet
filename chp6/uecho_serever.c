#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// #include <winsock2.h>
#define BUF_SIZE 1024
void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char* argv[]){
    int serv_sock;
    char buf[BUF_SIZE];
    int str_len;
    socklen_t clnt_adr_sz;
    struct sockaddr_in serv_adr,clnt_adr;
    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(serv_sock ==-1){
        error_handling("socket() error");
    }
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htonl(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1){
        error_handling("bind() error");
    }

    while(1){
        clnt_adr_sz = sizeof(clnt_adr);
        str_len = recvfrom(serv_sock,buf,BUF_SIZE,0,
                            (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        sendto(serv_sock,buf,str_len,0,
                (struct sockaddr*)&clnt_adr,clnt_adr_sz);
    }
    close(serv_sock);
    return 0;
}