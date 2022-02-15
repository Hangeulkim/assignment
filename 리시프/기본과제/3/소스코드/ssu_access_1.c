#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
    int i;

    if(argc <2){//체크할 파일의 인자가 있는지 확인
        fprintf(stderr,"usage %s <file1><file2>...<fileN>\n",argv[0]);
        exit(1);
    }

    for(i =1;i<argc;i++){//인자로 들어온 모든 파일을 확인
        if(access(argv[i],F_OK)<0){//argv[i]의 파일 존재여부 확인
            fprintf(stderr,"%s doesn't exist.\n",argv[i]);
            continue;
        }

        if(access(argv[i],R_OK)==0)//읽기 권한에 대해 확인
            printf("User can read %s\n",argv[i]);
        if(access(argv[i],W_OK)==0)//쓰기 권한에 대해 확인
            printf("User can write %s\n",argv[i]);
        if(access(argv[i],X_OK)==0)//실행 권한에 대해 확인
            printf("User can execute %s\n",argv[i]);
    }
        exit(0);
}

