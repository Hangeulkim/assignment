#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int main(void)
{
    sigset_t old_set;
    sigset_t sig_set;
    sigemptyset(&sig_set);//sig_set을 초기화
    sigaddset(&sig_set, SIGINT);//SIGINT를 sig_set에 추가
    sigprocmask(SIG_BLOCK, &sig_set, &old_set);//현재 마스크를 old_set에 저장시키고 sig_set을 블락시킴
    sigsuspend(&old_set);//블록시킬 시그널을 재설정
    exit(0);
}
