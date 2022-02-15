#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

int main(void)
{
    struct stat statbuf;
    char *fname="ssu_myfile";
    int fd;

    if((fd=open(fname,O_RDWR|O_CREAT,0600))<0){//SSU_MYFILE을 읽고 쓰기 제작 0600으로 open하여 fd에 저장
        fprintf(stderr,"open error for %s\n",fname);
        exit(1);
    }

    close(fd);
    stat(fname,&statbuf);//fname의 스탯 구조체를 statbuf에 저장
    printf("# 1st stat call # UID:%d    GID:%d\n",statbuf.st_uid,statbuf.st_gid);//ssu_myfile의 uid와 gid 출력

    if(chown(fname,501,300)<0){//심볼릭 링크가 가르키는 파일의 소유자 변경
        fprintf(stderr,"chown error for %s\n",fname);
        exit(1);
    }

    stat(fname, &statbuf);
    printf("# 2nd stat call # UID:%d    GID:%d\n",statbuf.st_uid,statbuf.st_gid);

    if(unlink(fname)<0){//fname을 제거
        fprintf(stderr,"unlink error for %s\n",fname);
        exit(1);
    }
    exit(0);
}
