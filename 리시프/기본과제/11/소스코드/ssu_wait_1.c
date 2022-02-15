#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

void ssu_echo_exit(int status);

int main(void)
{
    pid_t pid;
    int status;

    if((pid=fork())<0){//자식생성
        fprintf(stderr,"fork error\n");
        exit(1);
    }

    else if(pid==0)//자식을 7을 반환하고 종료시킴
        exit(7);

    if(wait(&status)!=pid){//자식이 종료되기를 기다림
        fprintf(stderr,"wait error\n");
        exit(1);
    }

    ssu_echo_exit(status);//status에 대한 정보를  echo

    if((pid=fork())<0){//자식생성
        fprintf(stderr,"fork error\n");
        exit(1);
    }
    else if(pid==0)//자식은 abort함수를 실행
        abort();

    if(wait(&status) != pid){//자식이 종료되길 기다림
        fprintf(stderr,"wait error\n");
        exit(1);
    }

    ssu_echo_exit(status);//status에 대한 정보를 보여줌

    if((pid=fork())<0){//자식을 생성
        fprintf(stderr,"fork error\n");
        exit(1);
    }
    else if(pid==0)//의도적으로 불가능한 연산을 시킴
        status/=0;

    if(wait(&status)!=pid){//자식이 종료되길 기다림
        fprintf(stderr,"wait error\n");
        exit(1);
    }

    ssu_echo_exit(status);//status 상태를 출력
    exit(0);
}

void ssu_echo_exit(int status){
    if(WIFEXITED(status))//자식이 정상적으로 종료되었는지를 따짐
        printf("normal termination, exit status = %d\n",WEXITSTATUS(status));
    else if(WIFSIGNALED(status))//자식 프로세스가 시그널을 받았으나 그것을 처리하지 않아 비정상적으로 종료됨
        printf("abnormal termination, signal number = %d%s\n",WTERMSIG(status),
#ifdef WCOREDUMP
            WCOREDUMP(status) ? " (core file generated)" : "");//코어파일이 생성되었는가를 따짐
#else
            "");
#endif
    else if(WIFSTOPPED(status))
        printf("child stopped, signal number = %d\n",WSTOPSIG(status));//자식프로세스가 현재 종료상태임을 알림
}
