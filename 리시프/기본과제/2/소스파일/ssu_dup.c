#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define BUFFER_SIZE 1024

int main(void){
    char buf[BUFFER_SIZE];
    char *fname = "ssu_test.txt";
    int count;
    int fd1, fd2;

    if((fd1 = open(fname,O_RDONLY,0644))<0){//ssu_test.txt를 읽기 0644로 open
        fprintf(stderr,"open error for %s\n",fname);
        exit(1);
    }

    fd2 = dup(fd1);//fd1을 fd2로 dup
    count = read(fd1,buf,12);//fd1을 12바이트 읽어옴
    buf[count] = 0;
    printf("fd1's printf : %s\n",buf);//읽어온 데이터 출력
    lseek(fd1,1,SEEK_CUR);//개행문자 넘김
    count = read(fd2, buf,12);//fd2의 12바이트 읽어옴
    buf[count] = 0;
    printf("fd2's printf : %s\n",buf);
    exit(0);
}

