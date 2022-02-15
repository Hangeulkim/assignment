#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[]){
    if(argc!=3){//인자가 사용법대로 들어오지 않았을 경우
        fprintf(stderr,"usage: %s <oldname> <newname>\n",argv[0]);
        exit(1);
    }

    if(link(argv[1],argv[2])<0){//argv[1]의 링크를 argv[2]에 생성
        fprintf(stderr,"link error\n");
        exit(1);
    }
    else
        printf("step1 passed.\n");

    if(remove(argv[1])<0){//argv[1]을 삭제
        fprintf(stderr,"remove error\n");
        remove(argv[2]);
        exit(1);
    }
    else
        printf("step2 passed.\n");

    printf("Success!\n");//전부 성공시
    exit(0);
}
