#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
static void ssu_signal_handler(int signo);
int main(void) {
    if (signal(SIGINT, ssu_signal_handler) == SIG_ERR) {//SIGINT가 들어올시 ssu_signal_handler을 실행하도록 설정
        fprintf(stderr, "cannot handle SIGINT\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGTERM, ssu_signal_handler) == SIG_ERR) {//SIGTERM가 들어올시 ssu_signal_handler을 실행하도록 설정
        fprintf(stderr, "cannot handle SIGTERM\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGPROF, SIG_DFL) == SIG_ERR) {//SIGPROF가 들어올시 기본 행동을 실행하도록 설정
        fprintf(stderr, "cannot reset SIGPROF\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {//SIGHUP가 들어올시 시그널 무시
        fprintf(stderr, "cannot ignore SIGHUP\n");
        exit(EXIT_FAILURE);
    }
    while (1)
        pause();
    exit(0);
}
static void ssu_signal_handler(int signo) {
    if (signo == SIGINT)//sigint가 들어올시 출력
        printf("caught SIGINT\n");
    else if (signo == SIGTERM)//sigterm 발생시 출력
        printf("caught SIGTERM\n");
    else {
        fprintf(stderr, "unexpected signal\n");
        exit(EXIT_FAILURE);//이상한 시그널이 들어와 실패하여 종료됨을 알림
    }
    exit(EXIT_SUCCESS);//성공으로 끝냈음을 알림
}
