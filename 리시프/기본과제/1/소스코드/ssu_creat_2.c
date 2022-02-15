#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>//close 사용을 위해 선언
#include<sys/stat.h>//사용하진 않으나 creat,open을 위해 선언
#include<sys/types.h>//사용하진 않으나 creat,open을 위해 선언
#include<fcntl.h>//파일을 만들때 권한 설정 및 open의 권한 설정을 위해 선언

int main(void){
    char *file_name="ssu_test.txt";//불러올 파일의 이름
    int fd;//불러온 파일의 디스크립터

    if((fd=creat(file_name,0666))<0){//파일이 0666권한으로 성공적으로 만들어 졌는지 판별
        fprintf(stderr,"creat error for %s\n",file_name);//에러 메시지
        exit(1);
    }
    else{
        close(fd);//만들어진 파일을 닫음
        fd = open(file_name,O_RDWR);//file_name을 읽기,쓰기 권한으로 엶
        printf("Succeeded!\n<%s> is new readable and writable\n",file_name);
    }

    exit(0);
}
