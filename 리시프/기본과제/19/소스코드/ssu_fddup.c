#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>

int main()
{
    int testfd;
    int fd;

    fd = open("test.txt",O_CREAT);

    testfd = fcntl(fd,F_DUPFD,5);//5를 인자로 넘겨 fd의 값을 5번 으로 복사
    printf("testfd :%d\n",testfd);
    testfd = fcntl(fd, F_DUPFD, 5);//이미 5번이 사용중이므로 6을 리턴
    printf("testfd :%d\n",testfd);

    getchar();
}
