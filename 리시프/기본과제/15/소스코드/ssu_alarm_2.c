#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define LINE_MAX 2048
static void ssu_alarm(int signo);
int main(void)
{
    char buf[LINE_MAX];
    int n;
    if (signal(SIGALRM, ssu_alarm) == SIG_ERR) {//SIGALRM발생시 ssu_alarm 실행
        fprintf(stderr, "SIGALRM error\n");
        exit(1);
    }
    alarm(10);//10초후 SIGALRM 발생
    if ((n = read(STDIN_FILENO, buf, LINE_MAX)) < 0) {//buf에 LINE_MAX만큼 읽어오고 길이를 n에 저장
        fprintf(stderr, "read() error\n");
        exit(1);
    }
    alarm(0);//즉시 SIGALRM 발생
    write(STDOUT_FILENO, buf, n);//BUF의 내용을 STDOUT_FILENO에 출력
    exit(0);
}

static void ssu_alarm(int signo) {
    printf("ssu_alarm() called!\n");//메시지 출력
}
