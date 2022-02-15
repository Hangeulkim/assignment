#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>

#define BUFFER_SIZE 1024

int main(int argc,char *argv[]){
    int length;
    char buf[BUFFER_SIZE];
    int fd1, fd2;

    if(argc!=3){
        fprintf(stderr,"please input file1 file2\n");
        exit(1);
    }

    if((fd1=open(argv[1],O_WRONLY|O_APPEND))<0){
        fprintf(stderr,"usage err %s\n",argv[1]);
        exit(1);
    }

    if((fd2=open(argv[2],O_RDONLY|O_CREAT))<0){
        fprintf(stderr,"usage err %s\n",argv[2]);
        exit(1);
    }

    while((length=read(fd2,buf,BUFFER_SIZE))>0){
        write(fd1,buf,length);
    }

    exit(0);
}
