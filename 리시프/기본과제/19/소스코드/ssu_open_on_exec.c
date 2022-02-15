#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

int main()
{
    int fd;
    int val;

    fd = open("exec_copy.txt",O_CREAT);//exec_copy.txt파일을 생성
    execl("/home/hangeulkim/리시프/과제19/loop","./loop",NULL);//loop를 실행
    exit(0);
}
