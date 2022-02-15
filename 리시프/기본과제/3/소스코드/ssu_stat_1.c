#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

int main(int argc, char *argv[]){
    struct stat statbuf;

    if(argc!=2){//읽어올 파일이 있는지 확인
        fprintf(stderr,"usage: %s <file>\n",argv[0]);
        exit(1);
    }

    if((stat(argv[1],&statbuf))<0){//argv[1]에 있는 파일의 stat 구조체를 buf에 리턴
        fprintf(stderr,"stat error\n");
        exit(1);
    }

    printf("%s is %ld bytes\n",argv[1],statbuf.st_size);
    exit(0);
}
