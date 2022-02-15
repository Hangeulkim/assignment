#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<utime.h>

int main(int argc, char *argv[])
{
    struct utimbuf time_buf;
    struct stat statbuf;
    int fd;
    int i;

    for(i=1;i<argc;i++){//인자로 들어온 파일 전부 체크
        if(stat(argv[i],&statbuf)<0){//argv[i]의 stat을 statbuf에 저장
            fprintf(stderr,"stat error for %s\n",argv[i]);
            continue;
        }

        if((fd=open(argv[i],O_RDWR|O_TRUNC))<0){//argv[i]를 읽고 쓰기 있을시 초기화로 open
            fprintf(stderr,"open error for %s\n",argv[i]);
            continue;
        }

        close(fd);//열었던 파일을 닫음
        time_buf.actime = statbuf.st_atime;//마지막 접근 시각 저장
        time_buf.modtime = statbuf.st_mtime;//마지막 수정 시각 저장

        if(utime(argv[i],&time_buf)<0){//argv[i]를 time_buf에 있는 시간으로 접근시각과 수정시각을 교체
            fprintf(stderr,"utime error for %s\n",argv[i]);
            continue;
        }

    }
    exit(0);
}
