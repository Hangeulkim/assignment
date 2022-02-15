#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main(void){
    char *fname = "ssu_test.txt";
    int fd;

    if ((fd = creat(fname,0666))<0){//ssu_test.txt를 0666권한으로 creat
        printf("creat error for %s\n",fname);
        exit(1);
    }

    printf("First printf is on the screen.\n");
    dup2(fd,1);//표준 출력을 파일 디스크립터로 사용
    printf("Second printf is in this file.\n");
    exit(0);
}
