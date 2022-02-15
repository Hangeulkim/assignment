#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo) {
    printf("ssu_signal_handler control\n");
}

int main(void) {
    struct sigaction sig_act;
    sigset_t sig_set;
    sigemptyset(&sig_act.sa_mask);//sig_act.sa_mask를 초기화 시킴
    sig_act.sa_flags = 0;//flag를 0으로 설정
    sig_act.sa_handler = ssu_signal_handler;//핸들러를 ssu_signal_handler로 설정
    sigaction(SIGUSR1, &sig_act, NULL);//SIGUSR1이 들어왔을때 sig_act를 하도록 설정
    printf("before first kill()\n");
    kill(getpid(), SIGUSR1);//현재 pid로 SIGUSR1을 전송
    sigemptyset(&sig_set);//sig_set을 초기화
    sigaddset(&sig_set, SIGUSR1);//sig_set에 SIGUSR1을 추가
    sigprocmask(SIG_SETMASK, &sig_set, NULL);//sig_set을 새 시그널 마스크로 설정
    printf("before second kill()\n");
    kill(getpid(), SIGUSR1);//SIGUSR1을 전송
    printf("after second kill()\n");
    exit(0);
}
