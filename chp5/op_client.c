#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// #include <winsock2.h>
#define BUF_SIZE 1024
#define INT_LEN sizeof(int)
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char*argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];
    int nums;
    char ans[4];
    int str_len = 0;

    if(argc != 3){
        prinf("Usage : %s <IP> <PORT>\n",argv[0]);
    }
    
    sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
        error_handling("connect() error");
    }

    printf("please input the nums of opnums:");
    scanf("%d",&nums);
    buf[0] = (char)nums;

    for(int i=0;i<nums;i++){
        printf("opnum%d is :",i+1);
        scanf("%d",(int *)buf[i*INT_LEN+1]);
    }
    getchar();
    printf("the operator is :");
    scanf("%c",buf[nums*INT_LEN+1]);

    if(write(sock,buf,nums*INT_LEN+2)==-1){
        error_handling("write() error");
    }

    int i = 0;
    int len;
    while(str_len<=INT_LEN){
        if((len = read(sock,ans[i++],1)) == -1){
            error_handling("read() error");
        }
        str_len += len;
    }
    printf("the ans is %d",*((int *)ans));

}