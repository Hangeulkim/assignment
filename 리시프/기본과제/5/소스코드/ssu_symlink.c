#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[]){
    if(argc!=3){//인자가 사용법대로 주어졌는지 체크
        fprintf(stderr,"usage: %s <actualname> <symname>\n",argv[0]);
        exit(1);
    }

    if(symlink(argv[1],argv[2])<0){//argv[1]의 심볼릭 링크를 argv[2]에 생성
        fprintf(stderr,"symlink error\n");
        exit(1);
    }
    else
        printf("symlink: %s -> %s\n",argv[2],argv[1]);
    exit(0);
}
