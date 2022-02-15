#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

struct stat statbuf;

void ssu_checkfile(char *fname, time_t *time);
int main(int argc, char *argv[])
{
    time_t intertime;
    if(argc!=2){//체크할 파일이 있는지 확인
        fprintf(stderr,"usage:%s <file>\n",argv[0]);
        exit(1);
    }

    if(stat(argv[1],&statbuf)<0){//argv[1]에 들어온 파일의 stat 구조체를 statbuf에 저장
        fprintf(stderr,"stat error for %s\n",argv[0]);
        exit(1);
    }

    intertime = statbuf.st_mtime;//statbyf의 최종 수정 시간을 저장
    while(1){
        ssu_checkfile(argv[1],&intertime);//argv[1]에 들어온 파일과 최종 수정 시간을 인자로 전달
        sleep(10);
    }
}

void ssu_checkfile(char *fname,time_t *time){
    if(stat(fname,&statbuf)<0){//인자로 들어온 파일의 stat구조체를 statbuf에 저장
        fprintf(stderr,"Warning:ssu_checkfile() error!\n");
        exit(1);
    }

    else
        if(statbuf.st_mtime!=*time){//들어온 인자의 시간과 최종 수정 시간이 다를경우
            printf("Warning : %s was modified!\n",fname);//파일이 수정되었음을 출력
            *time=statbuf.st_mtime;//인자를 최종 수정시간으로 변경
        }
}
