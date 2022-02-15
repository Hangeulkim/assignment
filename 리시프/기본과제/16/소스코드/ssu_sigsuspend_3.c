#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
void ssu_signal_handler(int signo);
void ssu_timestamp(char *str);
int main(void) {
    struct sigaction sig_act;
    sigset_t blk_set;
    sigfillset(&blk_set);//blk_set에 모든 signal요소를 추가
    sigdelset(&blk_set, SIGALRM);//blk_set에 SIGALRM을 제거
    sigemptyset(&sig_act.sa_mask);//sig_act.sa_mask를 초기화
    sig_act.sa_flags = 0;
    sig_act.sa_handler = ssu_signal_handler;//핸들러를 ssu_signal_handler로 설정
    sigaction(SIGALRM, &sig_act, NULL);//SIGALRM에 대한 액션을 sig_act로 설정
    ssu_timestamp("before sigsuspend()");
    alarm(5);
    sigsuspend(&blk_set);//시그널을 재설정
    ssu_timestamp("after sigsuspend()");
    exit(0);
}

void ssu_signal_handler(int signo) {
    printf("in ssu_signal_handler() function\n");
}

void ssu_timestamp(char *str) {
    time_t time_val;
    time(&time_val);
    printf("%s the time is %s\n", str, ctime(&time_val));//시간을 출력
}
