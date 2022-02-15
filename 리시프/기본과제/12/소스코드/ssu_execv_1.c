#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/wait.h>
double ssu_maketime(struct timeval *time);
void term_stat(int stat);
void ssu_print_child_info(int stat, struct rusage *rusage);
int main(void)
{
    struct rusage rusage;
    pid_t pid;
    int status;
    if ((pid = fork()) == 0) {
        char *args[] = {"find", "/", "-maxdepth", "4", "-name", "stdio.h", NULL};
        if (execv("/usr/bin/find", args) < 0) {//args를 인자로 넘겨주고 find 실행
            fprintf(stderr, "execv error\n");
            exit(1);
        }
    }
    if (wait3(&status, 0, &rusage) == pid)//자식이 끝나길 기다림
        ssu_print_child_info(status, &rusage);
    else {
        fprintf(stderr, "wait3 error\n");
        exit(1);
    }
    exit(0);
}

double ssu_maketime(struct timeval *time) {
    return ((double)time -> tv_sec + (double)time -> tv_usec/1000000.0);//시간을 초단위로 변경
}

void term_stat(int stat) {
    if (WIFEXITED(stat))
        printf("normally terminated. exit status = %d\n", WEXITSTATUS(stat));//정상적으로 종료됨
    else if (WIFSIGNALED(stat))
        printf("abnormal termination by signal %d. %s\n", WTERMSIG(stat),
    #ifdef WCOREDUMP
    WCOREDUMP(stat)?"core dumped":"no core"
    #else
    NULL
    #endif
);//종료할때 에러가 발생
    else if (WIFSTOPPED(stat))
        printf("stopped by signal %d\n", WSTOPSIG(stat));//중지됐을 경우
}

void ssu_print_child_info(int stat, struct rusage *rusage) {
    printf("Termination info follows\n");
    term_stat(stat);
    printf("user CPU time : %.2f(sec)\n", ssu_maketime(&rusage->ru_utime));//시간을 출력
    printf("system CPU time : %.2f(sec)\n", ssu_maketime(&rusage->ru_stime));//시간을 출력
}
