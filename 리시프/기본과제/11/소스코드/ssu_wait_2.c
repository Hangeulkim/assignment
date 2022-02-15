#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define EXIT_CODE 1
int main(void)
{
    pid_t pid;
    int ret_val, status;
    if ((pid = fork()) == 0) {//fork를 실행하여 자식일 경우 시행
        printf("child: pid = %d ppid = %d exit_code = %d\n",
        getpid(), getppid(), EXIT_CODE);//자식의 pid ppid exit_code를 출력하고 종료시킴
        exit(EXIT_CODE);
    }
    printf("parent: waiting for child = %d\n", pid);
    ret_val = wait(&status);//자식의 종료상태를 기다림
    printf("parent: return value = %d, ", ret_val);//종료 프로세스의 pid값을 가져옴
    printf(" child's status = %x", status);
    printf(" and shifted = %x\n", (status >> 8));
    exit(0);
}
