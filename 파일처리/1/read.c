#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    int length;
    int fd;
    char buf[BUFFER_SIZE];
    int off;
    int bytes;

    if(argc!=4){
        fprintf(stderr,"please input filepath offset read_bytes\n");
        exit(1);
    }

    off=atoi(argv[2]);
    bytes=atoi(argv[3]);

    if((fd=open(argv[1],O_RDONLY))<0){
        fprintf(stderr,"usage err %s\n",argv[1]);
        exit(1);
    }


    if(lseek(fd,off,SEEK_SET)<0){
        fprintf(stderr,"lseek failed\n");
        exit(1);
    }

    while((length=read(fd,buf,BUFFER_SIZE))>0&&bytes>0){
        if(length>bytes){
            write(1,buf,bytes);
        }
        else{
            write(1,buf,BUFFER_SIZE);
            bytes-=BUFFER_SIZE;
        }
    }
    exit(0);
}
