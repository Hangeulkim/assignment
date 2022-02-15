#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
void ssu_signal_handler(int signo);
jmp_buf jump_buffer;

int main(void) {
    signal(SIGINT, ssu_signal_handler);//SIGINT발생시 ssu_signal_handler를 실행
    while (1) {
        if (setjmp(jump_buffer) == 0) {//jump_buffer에 스택값들을 env에 저장, longjmp할 곳을 지정
            printf("Hit Ctrl-c at anytime ...\n");
            pause();
        }
    }
    exit(0);
}
void ssu_signal_handler(int signo) {
    char character;
    signal(signo, SIG_IGN);//signo발생시 무시하도록 바꿈
    printf("Did you hit Ctrl-c?\n" "Do you really want to quit? [y/n] ");
    character = getchar();
    if (character == 'y' || character == 'Y')
        exit(0);//y입력시 종료
    else {
        signal(SIGINT, ssu_signal_handler);//SIGINT발생시 ssu_signal_handler을 실행하도록 설정
        longjmp(jump_buffer, 1);//jump_buffer로 이동
    }
}
