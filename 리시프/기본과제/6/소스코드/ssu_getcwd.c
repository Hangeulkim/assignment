#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define PATH_MAX 1024

int main(void)
{
    char *pathname;

    if(chdir("/home/hangeulkim")<0){//작업 디렉토리를 변경
        fprintf(stderr,"chdir error\n");
        exit(1);
    }

    pathname = malloc(PATH_MAX);//PATH_MAX만큼 동적 할당
    if(getcwd(pathname,PATH_MAX)==NULL){//현재 작업 디렉토리에 대한 전체 경로 이름을 pathname에 저장
        fprintf(stderr,"getcwd error\n");
        exit(1);
    }
    printf("current directory=%s\n",pathname);
    exit(0);
}
