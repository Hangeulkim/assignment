#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#define MODE_EXEC (S_IXUSR|S_IXGRP|S_IXOTH)

int main(int argc, char *argv[]){
    struct stat statbuf;
    int i;
    if(argc<2){//들어온 인자가 1개 이하인지 체크
        fprintf(stderr,"usage : %s <file1> <file2> ... <fileN>\n",argv[0]);//프로그램 사용법 출력
        exit(1);
    }

    for(i=1;i<argc;i++){
        if(stat(argv[i],&statbuf)<0){//인자로 들어온 파일의 stat구조체를 statbuf에 저장
            fprintf(stderr,"%s : stat error\n",argv[i]);//스탯 구조체를 불러오는데 실패 했을시 다음껄로 진행
            continue;
        }

        statbuf.st_mode |= MODE_EXEC;//st.mode와 MODE_EXEC를 OR 연산후 st_mode에 할당
        statbuf.st_mode ^= (S_IXGRP|S_IXOTH);//st.mode와 (S_IXGRP|S_IXOTH)를 XOR연산 후 st_mode에 할당
        
        if(chmod(argv[i],statbuf.st_mode)<0)//argv[i]의 접근권한을 수정한 statbuf.st_mode로 변경
            fprintf(stderr,"%s : chmod error\n",argv[i]);
        else
            printf("%s : file permission was changed \n",argv[i]);

    }
    exit(0);
}
