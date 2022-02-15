#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static void ssu_charatatime(char *str);
int main(void)
{
    pid_t pid;
    if ((pid = fork()) < 0) {//fork 에러 처리
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid == 0)//자식인 경우
        ssu_charatatime("output from child\n");
    else//부모인 경우
        ssu_charatatime("output from parent\n");
    exit(0);
}
static void ssu_charatatime(char *str) {
    char *ptr;
    int print_char;
    setbuf(stdout, NULL);//buf를 stdout으로 설정
    for (ptr = str; (print_char = *ptr++) != 0; ) {
        putc(print_char, stdout);//str을 한글자씩  읽어와 stdout에 print_char을 출력
        usleep(10);
    }
}
