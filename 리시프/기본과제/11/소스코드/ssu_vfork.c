#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
void ssu_local_func(void);

int main(void)
{
    printf("Before vfork\n");
    ssu_local_func();
    printf("After ssu_local_func, my PID is %d\n", getpid());//�Լ��� ������ �� pid�� �����ͼ� ���
    exit(0);
}
void ssu_local_func(void) {
    pid_t pid;
    if ((pid = vfork()) == 0)//vfork�� ����
        printf("I'm child. My PID is %d\n", getpid());//�ڽ��� ��� �ڽ��� pid���
    else if (pid > 0)
        sleep(3);//�θ��ϰ�� ��� ������
    else
        fprintf(stderr, "vfork error\n");//vfork ����ó��
}
