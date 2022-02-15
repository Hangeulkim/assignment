#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
void ssu_signal_handler(int signo);
int count = 0;
int main(void)
{
    signal(SIGALRM, ssu_signal_handler);//SIGALRM발생시 ssu_signal_handler을 실행
    alarm(1);//1초후 SIGALRM 발생
    while(1);//무한 대기 시킴
    exit(0);
}

void ssu_signal_handler(int signo) {
    printf("alarm %d\n", count++);//count를 출력하고 1증가
    alarm(1);//1초후 SIGALRM 발생
}
