#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
void ssu_check_pending(int signo, char *signame);
void ssu_signal_handler(int signo);

int main(void)
{
    struct sigaction sig_act;
    sigset_t sig_set;
    sigemptyset(&sig_act.sa_mask);//sig_act.sa_mask를 초기화
    sig_act.sa_flags = 0;
    sig_act.sa_handler = ssu_signal_handler;//핸들러를 ssu_signal_handler로 설정
    if (sigaction(SIGUSR1, &sig_act, NULL) != 0) {//SIGUSR1이 발생시 sig_act를 실행
        fprintf(stderr, "sigaction() error\n");
        exit(1);
    }

    else {
        sigemptyset(&sig_set);//sig_set을 초기화
        sigaddset(&sig_set, SIGUSR1);//SIGUSR1을 추가
        if (sigprocmask(SIG_SETMASK, &sig_set, NULL) != 0) {//sig_set에 마스크를 설정
            fprintf(stderr, "sigprocmask() error\n");
            exit(1);
        }

        else {
            printf("SIGUSR1 signals are now blocked\n");
            kill(getpid(), SIGUSR1);//SIGUSR1을 발생시킴
            printf("after kill()\n");
            ssu_check_pending(SIGUSR1, "SIGUSR1");
            sigemptyset(&sig_set);//sig_set을 초기화
            sigprocmask(SIG_SETMASK, &sig_set, NULL);//sig_set에 마스크를 설정
            printf("SIGUSR1 signals are no longer blocked\n");
            ssu_check_pending(SIGUSR1, "SIGUSR1");
        }
    }
    exit(0);
}

void ssu_check_pending(int signo, char *signame) {
    sigset_t sig_set;
    if (sigpending(&sig_set) != 0)//sig_set을 pending
        printf("sigpending() error\n");
    else if (sigismember(&sig_set, signo))//signo가 sig_set에 멤버로 있는지 확인
        printf("a %s signal is pending\n", signame);
    else
        printf("%s signals are not pending\n", signame);
}

void ssu_signal_handler(int signo) {
    printf("in ssu_signal_handler function\n" );
}
