#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define size 10

int main(){
    int fd;
    char buf[size];
    fd = open("data.txt",O_RDONLY);
    if(fd == -1){
        printf("open error\n");
    }
    int a;
    while((a=read(fd,buf,sizeof(buf)))!=0){
        // buf[size-1] = '\0';
        printf("%s",buf);
        printf("%d\n",a);
    }
    close(fd);
    return 0;
}