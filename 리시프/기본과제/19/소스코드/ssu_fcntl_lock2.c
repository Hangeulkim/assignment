#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
    struct flock lock;
    int fd;
    char command[100];

    if((fd=open(argv[1],O_RDWR))==-1){//인자로 들어온 파일을 읽고 쓰기로 open
        perror(argv[1]);
        exit(1);
    }
    lock.l_type=F_WRLCK;//쓰기 lock을 설정
    lock.l_whence=0;
    lock.l_start=0l;
    lock.l_len=0l;
    if(fcntl(fd,F_SETLK,&lock)==-1){//fd 에 lock을 설정
        if(errno == EACCES){
            printf("%s busy -- try later\n",argv[1]);
            exit(2);
        }
        perror(argv[1]);
        exit(3);
    }

    sprintf(command,"vim %s\n",argv[1]);//command에 입력
    system(command);//command실행
    lock.l_type = F_UNLCK;//unlck로 변경
    fcntl(fd,F_SETLK,&lock);//lock 재설정
    close(fd);

    return 0;
}
