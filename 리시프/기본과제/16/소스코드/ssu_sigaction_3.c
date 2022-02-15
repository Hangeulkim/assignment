#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
static void ssu_signal_handler1(int signo);
static void ssu_signal_handler2(int signo);

int main(void) {
    struct sigaction act_int, act_quit;
    act_int.sa_handler = ssu_signal_handler1;//핸들러를 ssu_signal_handler1로 설정
    sigemptyset(&act_int.sa_mask);//act_int.sa_mask를 초기화
    sigaddset(&act_int.sa_mask, SIGQUIT);//SIGQUIT을 추가
    act_quit.sa_flags = 0;
    if (sigaction(SIGINT, &act_int, NULL) < 0) {//SIGINT발생시 act_int를 실행
        fprintf(stderr, "sigaction(SIGINT) error\n");
        exit(1);
    }
    act_quit.sa_handler = ssu_signal_handler2;//핸들러를 ssu_signal_handler2로 설정
    sigemptyset(&act_quit.sa_mask);//마스크를 초기화
    sigaddset(&act_quit.sa_mask, SIGINT);//SIGINT를 추가시킴
    act_int.sa_flags = 0;
    if (sigaction(SIGQUIT, &act_quit, NULL) < 0) {//SIGQUIT발생시 act_quit실행
        fprintf(stderr, "sigaction(SIGQUIT) error\n");
        exit(1);
    }
    pause();
    exit(0);
}

static void ssu_signal_handler1(int signo) {
    printf("Signal handler of SIGINT : %d\n", signo);
    printf("SIGQUIT signal is blocked : %d\n", signo);
    printf("sleeping 3 sec\n");
    sleep(3);
    printf("Signal handler of SIGINT ended\n");
}

static void ssu_signal_handler2(int signo) {
    printf("Signal handler of SIGQUIT : %d\n", signo);
    printf("SIGINT signal is blocked : %d\n", signo);
    printf("sleeping 3 sec\n");
    sleep(3);
    printf("Signal handler of SIGQUIT ended\n");
}
