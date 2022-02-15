#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int main(void)
{
    sigset_t pendingset;
    sigset_t sig_set;
    int count = 0;
    sigfillset(&sig_set);//sig_set에 모든 시그널을 포함시킴
    sigprocmask(SIG_SETMASK, &sig_set, NULL);//sig_set에 이전 블록된 시그널 집합을 모두 지우고 sig_set이 가리키는 시그널 집합의 값을 새 시그널 마스크로 설정
    while (1) {
        printf("count: %d\n", count++);
        sleep(1);
        if (sigpending(&pendingset) == 0) {//펜딩 중이거나 블록되었을때 현재 팬딩중인 시그널들의 집합을 pendingset에 저장
            if (sigismember(&pendingset, SIGINT)) {//pendingset에 SIGINT가 있는지 확인
                printf("SIGINT가 블록되어 대기 중. 무한 루프를 종료.\n");
                break;
            }
        }
    }
    exit(0);
}
