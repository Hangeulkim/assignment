#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void)
{
    if (fork() == 0)//자식일 경우 실행
        execl("/bin/echo", "echo", "this is", "message one", (char *)0);//위의 명령어를 실행

    if (fork() == 0)//자식일 경우 실행
        execl("/bin/echo", "echo", "this is", "message Two", (char *)0);//위의 명령어를 실행

    printf("parent: waiting for children\n");

    while (wait((int *)0) != -1)//자식이 끝나길 기다림
        ;
    
    printf("parent: all children terminated\n");//자식이 끝났음을 알림
    exit(0);
}
