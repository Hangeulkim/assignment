#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
    if(argc<3){//인자가 올바르게 들어왔는지 체크
        fprintf(stderr,"usage: %s <file1> <file2>\n",argv[0]);
        exit(1);
    }

    if(link(argv[1],argv[2])==-1){//argv[1]에 대한 링크 설정
        fprintf(stderr,"link error for %s\n",argv[1]);
        exit(1);
    }
    exit(0);
}
