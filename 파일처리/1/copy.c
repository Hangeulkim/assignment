#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

#define BUFFER_SIZE 100

int main(int argc,char *argv[]){
    char buf[BUFFER_SIZE];
    int length;
    int fd1,fd2;

    if(argc!=3){
        fprintf(stderr,"please input 2 files\n");
        exit(1);
    }

    if((fd1=open(argv[1],O_RDONLY))<0){
        fprintf(stderr,"usage err %s\n",argv[1]);
        exit(1);
    }

    if((fd2=creat(argv[2],0666))<0){
        fprintf(stderr,"usage err %s\n",argv[2]);
        exit(1);
    }

    while((length=read(fd1,buf,BUFFER_SIZE))>0){
        write(fd2,buf,length);
    }

    exit(0);
}
