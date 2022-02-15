#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(void)
{
    pid_t child1, child2;
    int pid,status;

    if((child1 = fork())==0)//�ڽ�1�� ����
        execlp("date","date",(char*)0);//date��� ����

    if((child2=fork())==0)//�ڽ� 2�� ����
        execlp("who","who",(char*)0);//who��ɾ� ����

    printf("parent: waiting for children\n");

    while((pid=wait(&status))!=-1){//�ڽ��� ����Ǳ� ��ٸ�
        if(child1==pid)//�ڽ� 1�� ������� �˸�
            printf("parent: first child: %d\n",(status >> 8));
        else if(child2==pid)//�ڽ� 2�� ������� �˸�
            printf("parent: second child: %d\n", (status >> 8));
    }

    printf("parent: all children terminated\n");//��� �ڽ��� Ȱ���� �����
    exit(0);
}
