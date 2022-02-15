#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
void ssu_alarm(int signo);
int main(void) {
    printf("Alarm Setting\n");
    signal(SIGALRM, ssu_alarm);//SIGALRM발생시 ssu_alarm 실행
    alarm(2);//2초후 SIGALRM 발생
    while (1) {
        printf("done\n");
        pause();//시그널이 올때까지 멈춤
        alarm(2);//2초후 SIGALRM 발생
    }
    exit(0);
}

void ssu_alarm(int signo) {
    printf("alarm..!!!\n");//메시지 출력
}
