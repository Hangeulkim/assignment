#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>

#define MSG "message will be written to Terminal\n"

int main()
{
    int new_fd;

    if((new_fd=fcntl(STDOUT_FILENO,F_DUPFD,3))==-1){//STDOUT_FILENO를 3번으로 복사
        fprintf(stderr,"Error : Copying File Descriptor\n");
        exit(1);
    }

    close(STDOUT_FILENO);
    write(3,MSG,strlen(MSG));//3번에 msg를 기록
    exit(0);
}
