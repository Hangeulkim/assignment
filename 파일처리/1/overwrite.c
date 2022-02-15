#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>


int main(int argc, char *argv[]){
    int length;
    int fd;
    int size;

    if(argc!=4){
        fprintf(stderr,"please input filepath offset date\n");
        exit(1);
    }

    int off=(atoi(argv[2]));

    if((fd=open(argv[1],O_RDWR))<0){
        fprintf(stderr,"usage err %s\n",argv[1]);
        exit(1);
    }

    if(lseek(fd,(off_t)off,SEEK_SET)<0){
        fprintf(stderr,"lseek failed\n");
        exit(1);
    }

    if(write(fd,argv[3],strlen(argv[3]))<0){
        fprintf(stderr,"write err\n");
        exit(1);
    }
    
    exit(0);
}
