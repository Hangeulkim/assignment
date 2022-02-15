#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

int main()
{
    int fd;
    int val;

    if((fd=open("exec_copy.txt",O_CREAT))<0){//exec_copy.txt를 open
        fprintf(stderr, "open error for %s\n","exec_copy.txt");
        exit(1);
    }
    val = fcntl(fd,F_GETFD,0);//fd의 플래그를 가져와서 val에 저장

    if(val & FD_CLOEXEC)//FD_CLOEXEC이 설정됐는지 체크
        printf("close-on-exec bit on\n");
    else
        printf("close-on-exec bit off\n");

    val |= FD_CLOEXEC;//FD_CLOEXEC를 설정

    if(val & FD_CLOEXEC)//FD_CLOEXEC이 설정됐는지 체크
        printf("close-on-exec bit on\n");
    else
        printf("close-on-exec bit off\n");

    fcntl(fd, F_SETFD,val);//fd의 플래그를 val로 설정
    execl("/home/hangeulkim/loop","./loop",NULL);//loop실행
}
