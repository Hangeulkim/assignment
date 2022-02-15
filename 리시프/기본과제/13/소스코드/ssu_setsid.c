#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    pid_t pid;
    if ((pid = fork()) < 0) {//자식 생성 에러체크
    fprintf(stderr, "fork error\n");
    exit(1);
    }
    else if(pid != 0)//부모는 종료
        exit(1);
    printf("before pid = %d, sid = %d\n", getpid(), getsid(getpid()));//pid와 sid를 출력
    setsid();//새로운 세션을 생성
    printf("after pid = %d, sid = %d\n", getpid(), getsid(getpid()));//바뀐 pid와 sid 새로 출력
    exit(0);
}
