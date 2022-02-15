#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define S_MODE 0644
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    char buf[BUFFER_SIZE];
    int fd1, fd2;
    int length;

    if(argc != 3){//읽어올파일 저장할파일 이름이 있는지 확인
        fprintf(stderr, "Usage : %s filein fileout\n",argv[0]);
        exit(1);
    }

    if((fd1=open(argv[1],O_RDONLY))<0){//1번의 파일을 읽기 권한으로 open
        fprintf(stderr,"open error for %s\n",argv[1]);
        exit(1);
    }

    if((fd2=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,S_MODE))<0){//2번의 파일을 쓰기,제작,있을경우 초기화 0644권한으로 open
        fprintf(stderr,"open error for %s\n",argv[2]);
        exit(1);
    }

    while((length = read(fd1,buf,BUFFER_SIZE))>0)
        write(fd2,buf,length);//fd1을 fd2에 내용을 씀

    exit(0);
}
