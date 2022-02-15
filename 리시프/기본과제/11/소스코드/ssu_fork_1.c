#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
char glob_str[] = "write to standard output\n";
int glob_val = 10;
int main(void)
{
    pid_t pid;
    int loc_val;
    loc_val = 100;
    if (write(STDOUT_FILENO,glob_str, sizeof(glob_str)-1) != sizeof(glob_str) - 1) {//glob_str의 끝에 도달할때까지 stdout에 glob_str을 작성
        fprintf(stderr, "write error\n");
        exit(1);
    }
    printf("before fork\n");
    if ((pid = fork()) < 0) {//fork가 제대로 되었는지 확인
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid == 0) {//부모 프로세스의 경우 glob_val과 loc_val을 증가시킴
        glob_val++;
        loc_val++;
    }
    else
        sleep(3);
    printf("pid = %d, glob_val = %d, loc_val = %d\n",getpid(), glob_val, loc_val);//pid와 glob_val과 loc_val을 출력
    exit(0);
}
