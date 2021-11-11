#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
// #include <winsock2.h>
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[]){
    int sock_serv;
    int sock_clnt;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    char buf[1024];
    int rec_len = 0;
    int ans;

    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        return 0;
    }
    sock_serv = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock_serv == -1){
        error_handling("socket() error");
    }
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock_serv,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1){
        error_handling("band() error");
    }
    if(listen(sock_serv,5) == -1){
        error_handling("listen() error");
    }

    int size_addr = sizeof(clnt_addr);
    for(int i=0;i<5;i++){
        if((sock_clnt = accept(sock_serv,(struct sockaddr*)&clnt_addr,&size_addr)) == -1){
            error_handling("accept() error");
        }
        else{
            printf("accept successfully\n");
        }
        int str_len = 0;
        while(rec_len<1){
            if((str_len = read(sock_clnt,&buf[rec_len],1)) == -1){
                error_handling("read() error");
            }
            rec_len+=str_len;
        }
        printf("the num of opnums is %d\n",(int)buf[0]);
        int *nums = (int *)malloc((int)buf[0]*sizeof(int));
        for(int i=0;i<(int)buf[0];i++){
            while(rec_len<1+(i+1)*4){
                if((str_len = read(sock_clnt,&buf[rec_len],4))==-1){
                    error_handling("read() error");
                }
                rec_len += str_len;
            }
            nums[i] = (*(int*)(&buf[i*4+1]));
            printf("opnum %d :%d\n",i+1,nums[i]);
        }
        while(rec_len<1+4*(int)buf[0]+1){
            if((str_len = read(sock_clnt,&buf[rec_len],1)) == -1){
                error_handling("read() error");
            }
            rec_len += str_len;
        }
        printf("the operator is %c",buf[rec_len-1]);
        if(buf[rec_len-1] == '+'){
            for(int i=0;i<(int)buf[0];i++){
                ans+=nums[i];
            }
        }
        if(write(sock_clnt,&ans,sizeof(int))==-1){
            error_handling("write() error");
        }
        close(sock_clnt);
        free(nums);
    }
    close(sock_serv);
}