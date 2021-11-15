#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
// #include <winsock2.h>

#define BUF_SIZE 1024
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(0);
}

int main(int argc,char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    int read_cnt;
    char buf[BUF_SIZE];
    FILE *fp;
    fp = fopen("./receive.dat","wb+");

    if(argc != 3){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(argv[2]);

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
        error_handling("connect() error");
    }
    while((read_cnt = read(sock,buf,BUF_SIZE))!=0){
        fwrite(buf,1,read_cnt,fp);
    }

    fputs("receive data\n",stdout);
    write(sock,"thank you",10);
    fclose(fp);
    close(sock);
    return 0;
}