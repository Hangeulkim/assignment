#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>//write, lseek 사용을 위함
#include<fcntl.h>//creat 사용을 위함
#include<sys/types.h>//creat, lseek 사용을 위함
#include<sys/stat.h>//creat 사용을 위함

#define CREAT_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)//생성할 파일의 권한 설정을 간단히 하기위해 선언 00644

char buf1[]="1234567890";
char buf2[]="ABCDEFGHU";

int main(void){
    char *file_name="ssu_hole.txt";//만들 파일명
    int fd;//파일의 디스크립터

    if((fd = creat(file_name, CREAT_MODE))<0){//파일을 만듦
        fprintf(stderr,"creat error for %s\n",file_name);//에러 메시지
        exit(1);
    }

    if(write(fd,buf1,12)!=12){//buf1의 내용을 열려있는 파일에 씀
        fprintf(stderr,"buf1 write error\n");//에러 메시지
        exit(1);
    }

    if(lseek(fd,15000,SEEK_SET)<0){//오프셋 위치를 15000만큼 옮김
        fprintf(stderr,"lseek error\n");//에러 메시지
        exit(1);
    }

    if(write(fd,buf2,12)!=12){//오프셋 위치에 buf2의 내용을 씀
        fprintf(stderr,"buf2 write error\n");//에러 메시지
        exit(1);
    }

    exit(0);
}
