#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

#define BUFFER_SIZE 1024

int main(int argc,char *argv[]){
    int length;
    int fd;
    char buf[BUFFER_SIZE];
    char *temp_file="temp";
    int temp_fd;
    int off;
    int delete_bytes;
    off_t size;

    if(argc!=4){
        fprintf(stderr,"please input filepath offset delete_bytes\n");
        exit(1);
    }

    if((fd=open(argv[1],O_RDWR))<0){
        fprintf(stderr,"usage err %s\n",argv[1]);
        exit(1);
    }

    temp_fd=creat(temp_file,0666);
    off=atoi(argv[2]);
    delete_bytes=atoi(argv[3]);

    int tmp_off=off;
    while(tmp_off>0){
        if(tmp_off>BUFFER_SIZE){
            length=read(fd,buf,BUFFER_SIZE);
            write(temp_fd,buf,length);
        }
        else{
            length=read(fd,buf,tmp_off);
            write(temp_fd,buf,length);
        }
        tmp_off-=BUFFER_SIZE;
    }

    if((size=lseek(fd,(off_t)0,SEEK_END))>(long)off+delete_bytes){
        lseek(fd,off+delete_bytes,SEEK_SET);
        while((length=read(fd,buf,BUFFER_SIZE))>0)
            write(temp_fd,buf,length);
    }

    close(temp_fd);
    temp_fd=open(temp_file,O_RDONLY);
    close(fd);
    fd=open(argv[1],O_WRONLY|O_TRUNC);
    while((length=read(temp_fd,buf,BUFFER_SIZE))>0)
        write(fd,buf,length);
    close(fd);        
    close(temp_fd);
    remove(temp_file);

    exit(0);
}
