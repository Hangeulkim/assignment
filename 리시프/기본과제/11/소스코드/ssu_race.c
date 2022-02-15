#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static void ssu_charatatime(char *str);
int main(void)
{
    pid_t pid;
    if ((pid = fork()) < 0) {//fork ���� ó��
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid == 0)//�ڽ��� ���
        ssu_charatatime("output from child\n");
    else//�θ��� ���
        ssu_charatatime("output from parent\n");
    exit(0);
}
static void ssu_charatatime(char *str) {
    char *ptr;
    int print_char;
    setbuf(stdout, NULL);//buf�� stdout���� ����
    for (ptr = str; (print_char = *ptr++) != 0; ) {
        putc(print_char, stdout);//str�� �ѱ��ھ�  �о�� stdout�� print_char�� ���
        usleep(10);
    }
}
