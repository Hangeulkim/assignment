#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main()
{
    char *filename="ssu_test.txt";
    int fd1, fd2;
    int flag;

    if((fd1=open(filename,O_RDWR | O_APPEND,0644))<0){//filename을 읽고 쓰기로 추가 0644로 open하여 fd1로 연결
        fprintf(stderr, "open error for %s\n", filename);
        exit(1);
    }

    if(fcntl(fd1, F_SETFD, FD_CLOEXEC)==-1){//fd1의 플래그를 FD_CLOEXEC로 설정
        fprintf(stderr, "fcntl F_SETFD error\n");
        exit(1);
    }

    if((flag=fcntl(fd1,F_GETFL,0))==-1){//fd1의 파일 상태 플래그를 읽어와서 flag에 저장
        fprintf(stderr,"fcntl F_GETFL error\n");
        exit(1);
    }

    if(flag & O_APPEND)//O_APPEND가 설정돼있는지 체크
        printf("fd1 : O_APPEND flag is set.\n");
    else
        printf("fd1 : O_APPEND flag is NOT set.\n");

    if((flag = fcntl(fd1,F_GETFD,0))==-1){//fd1의 fd플래그를 가져옴
        fprintf(stderr, "fcntl F_GETFD error\n");
        exit(1);
    }

    if(flag & FD_CLOEXEC)//cloexec이 설정 돼있는지 체크
        printf("fd1 : FD_CLOEXEC flag is set.\n");
    else
        printf("fd1 : FD_CLOEXEC flag is NOT set.\n");



    if((fd2=fcntl(fd1, F_DUPFD, 0))==-1){//fd1을 복제하여 fd2에 저장
        fprintf(stderr, "fcntl F_SETFD error\n");
        exit(1);
    }

    if((flag=fcntl(fd2,F_GETFL,0))==-1){//fd2의 파일 상태 플래그를 읽어옴
        fprintf(stderr,"fcntl F_GETFL error\n");
        exit(1);
    }

    if(flag & O_APPEND)//append가 설정 돼있는지 확인
        printf("fd2 : O_APPEND flag is set.\n");
    else
        printf("fd2 : O_APPEND flag is NOT set.\n");

    if((flag = fcntl(fd2,F_GETFD,0))==-1){//fd2의 fd플래그를 가져옴
        fprintf(stderr, "fcntl F_GETFD error\n");
        exit(1);
    }

    if(flag & FD_CLOEXEC)//cloexec가 설정 됐는지 체크
        printf("fd2 : FD_CLOEXEC flag is set.\n");
    else
        printf("fd2 : FD_CLOEXEC flag is NOT set.\n");

    exit(0);
}
