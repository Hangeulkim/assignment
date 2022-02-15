#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>

#define BUFFER_SIZE 1024

int main(int argc,char *argv[]){
    int length;
    char buf[BUFFER_SIZE];
    int fd;
    int temp_fd;
    char *temp_file="temp";

    if(argc!=4){
        fprintf(stderr,"please input filepath offset data\n");
        exit(1);
    }

    int off=atoi(argv[2]);
    temp_fd=creat(temp_file,0666);

    if((fd=open(argv[1],O_RDWR))<0){
        fprintf(stderr,"usage err %s\n",argv[1]);
        exit(1);
    }

    if(lseek(fd,off,SEEK_SET)<0){
       fprintf(stderr,"lseek err\n");
       exit(1);
    }
    

    while((length=read(fd,buf,BUFFER_SIZE))>0){
        write(temp_fd,buf,length);
    }

    lseek(fd,off,SEEK_SET);
    write(fd,argv[3],strlen(argv[3]));

    close(temp_fd);
    temp_fd=open(temp_file,O_RDONLY);
    while((length=read(temp_fd,buf,BUFFER_SIZE))>0)
        write(fd,buf,length);
    

   close(temp_fd);
   remove(temp_file);
   close(fd);

   exit(0);
}
