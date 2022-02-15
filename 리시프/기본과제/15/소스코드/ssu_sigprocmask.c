#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int main(void)
{
    sigset_t sig_set;
    int count;
    sigemptyset(&sig_set);//sig_set을 초기화
    sigaddset(&sig_set, SIGINT);//sig_set에 SIGINT를 추가
    sigprocmask(SIG_BLOCK, &sig_set, NULL);//sig_set에 마스크를 SIG_BLOCK를 설정
    for (count = 3 ; 0 < count ; count--) {
        printf("count %d\n", count);
        sleep(1);
    }
    printf("Ctrl-C에 대한 블록을 해제\n");
    sigprocmask(SIG_UNBLOCK, &sig_set, NULL);//sig_set에 마스크 설정한 블록을 해제
    printf("count중 Ctrl-C입력하면 이 문장은 출력 되지 않음.\n");
    while (1);
    exit(0);
}
