#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define TABLE_SIZE (sizeof(table)/sizeof(*table))

int main(int argc, char *argv[])
{
    struct{//파일의 모드를 편하게 확인하기 위해 선언
        char *text;
        int mode;
    } table[] = {
        {"exists",0},
        {"execute",1},
        {"write",2},
        {"read",4}
    };


    int i;
    if(argc<2){//체크할 파일의 인자가 있는지 확인
        fprintf(stderr,"usage:%s <file>\n",argv[0]);
        exit(1);
    }

    for(i=0;i<TABLE_SIZE;i++){//들어온 파일의 exists,execute,write,read권한 확인
        if(access(argv[1],table[i].mode)!=-1)//만들어논 테이블 배열과 들어온 파일의 권한 확인
            printf("%s -ok\n",table[i].text);
        else
            printf("%s\n",table[i].text);
    }
    exit(0);
}
