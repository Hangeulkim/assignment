#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<signal.h>
#include<termios.h>

#define STDIN_NUMBER 0

static void ssu_sig_hup(int signum);
static void ssu_print_ids(char *name);

int main(void)
{
    pid_t pid;
    char character;

    ssu_print_ids("parent");//부모의 id들 출력
    if((pid=fork())<0){//fork 에러체크
        fprintf(stderr,"fork error\n");
        exit(1);
    }
    else if(pid > 0){//부모일경우 잠시 멈췄다가 종료
        sleep(3);
        exit(0);
    }
    else{
        ssu_print_ids("child");//자식의 id출력
        signal(SIGHUP,ssu_sig_hup);
        kill(getpid(),SIGTSTP);
        ssu_print_ids("child");//signal을 끊은 뒤의 id 출력

        if(read(STDIN_FILENO,&character,1)!=1)//에러발 유도
            fprintf(stderr, "read error\n");

        exit(0);
    }
}

static void ssu_sig_hup(int signum){
    printf("ssu_SIG_HUP received, pid = %d\n",getpid());
}

static void ssu_print_ids(char *name){
    printf("[%s] : pid = %d, ppid = %d, pgrp = %d, tpgrp = %d\n",name,getpid(),getppid(),getpgrp(),tcgetpgrp(STDIN_NUMBER));//name,pid,ppid,pgrp,전경 프로세스 그룹 출력
    fflush(stdout);
}
