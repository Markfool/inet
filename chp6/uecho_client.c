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
    exit(0);
}

int main(int argc,char* argv[]){
    int sock;
    char buf[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;
    struct sockaddr_in serv_adr,clnt_adr;

    if(argc != 3){
        printf("Usage : %s <ip> <port>\n",argv[0]);
        exit(1);
    }
    sock = socket(PF_INET,SOCK_DGRAM,0);
    if(sock == -1){
        error_handling("socket() error");
    }
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));
    while(1){
        fputs("Input ,q to quit : \n",stdout);
        fgets(buf,BUF_SIZE,stdin);
        if(!strcmp(buf,"q\n")){
            break;
        }
        sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&serv_adr,sizeof(serv_adr));
        adr_sz = sizeof(serv_adr);
        str_len = recvfrom(sock,buf,BUF_SIZE,0,(struct sockaddr*)&serv_adr,&adr_sz);
        buf[str_len] = '\0';
        printf("Message from Server is : %s",buf);

    }
    return 0;
}
