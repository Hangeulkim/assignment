#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void)
{
    if (fork() == 0)//�ڽ��� ��� ����
        execl("/bin/echo", "echo", "this is", "message one", (char *)0);//���� ��ɾ ����

    if (fork() == 0)//�ڽ��� ��� ����
        execl("/bin/echo", "echo", "this is", "message Two", (char *)0);//���� ��ɾ ����

    printf("parent: waiting for children\n");

    while (wait((int *)0) != -1)//�ڽ��� ������ ��ٸ�
        ;
    
    printf("parent: all children terminated\n");//�ڽ��� �������� �˸�
    exit(0);
}
