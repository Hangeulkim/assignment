#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int main(void)
{
    char buf[64];
    char *fname="ssu_tempfile";
    int fd;
    int length;
    if((fd=open(fname,O_RDWR|O_CREAT|O_TRUNC,0600))<0){//fname을 0600으로 읽고 쓰기 없으면 만들고 있을시 초기화로 open
        fprintf(stderr,"file open error for %s\n",fname);
        exit(1);
    }

    if(unlink(fname)<0){//fname을 삭제
        fprintf(stderr,"unlink error for %s\n",fname);
        exit(1);
    }

    if(write(fd,"How are you?",12)!=12){//fd에 How are you?를 write
        fprintf(stderr,"write error\n");
        exit(1);
    }

    lseek(fd,0,0);//처음으로 이동시킴
    if((length=read(fd,buf,sizeof(buf)))<0){//fd를 읽어와서 buf에 저장
        fprintf(stderr,"buf read error\n");
        exit(1);
    }
    else
        buf[length]=0;//버퍼의 마지막을 널문자로 설정

    printf("%s\n",buf);

    close(fd);//fd를 닫음

    if((fd=open(fname,O_RDWR))<0){//fname을 읽고 쓰기로 다시 open
        fprintf(stderr,"second open error %s\n",fname);
        exit(1);
    }

    else
        close(fd);
    exit(0);
}
