#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(void)
{
    pid_t child1, child2;
    int pid,status;

    if((child1 = fork())==0)//자식1을 실행
        execlp("date","date",(char*)0);//date명령 실행

    if((child2=fork())==0)//자식 2를 실행
        execlp("who","who",(char*)0);//who명령어 실행

    printf("parent: waiting for children\n");

    while((pid=wait(&status))!=-1){//자식이 종료되길 기다림
        if(child1==pid)//자식 1이 종료됨을 알림
            printf("parent: first child: %d\n",(status >> 8));
        else if(child2==pid)//자식 2가 종료됨을 알림
            printf("parent: second child: %d\n", (status >> 8));
    }

    printf("parent: all children terminated\n");//모든 자식의 활동이 종료됨
    exit(0);
}
