#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
void ssu_signal_handler1(int signo);
void ssu_signal_handler2(int signo);
int main(void)
{
    if (signal(SIGINT, ssu_signal_handler1) == SIG_ERR) {//SIGINT가 들어올시 ssu_signal_handler1을 실행하게 만듬
        fprintf(stderr, "cannot handle SIGINT\n");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGUSR1, ssu_signal_handler2) == SIG_ERR) {//SIGUSR1가 들어올시 ssu_signal_handler2을 실행하게 만듬
        fprintf(stderr, "cannot handle SIGUSR1\n");
        exit(EXIT_FAILURE);
    }
    raise(SIGINT);//SIGINT를 발생시킴
    raise(SIGUSR1);//SIGUSR1을 발생시킴
    printf("main return\n");
    exit(0);
}

void ssu_signal_handler1(int signo) {
    printf("SIGINT 시그널 발생\n");
}

void ssu_signal_handler2(int signo) {
    printf("SIGUSR1 시그널 발생\n");
}
