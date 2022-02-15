#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<syslog.h>
#include<sys/stat.h>
#include<sys/types.h>

int ssu_daemon_init(void);

int main(void)
{
    printf("daemon process initialization\n");

    if(ssu_daemon_init() < 0){//ssu_daemon_init실행
        fprintf(stderr, "ssu_daemon_init failed\n");
        exit(1);
    }

    while(1){//5초마다 로그 메시지를 남김
        openlog("lpd",LOG_PID,LOG_LPR);
        syslog(LOG_ERR,"open failed lpd %m");
        closelog();

        sleep(5);
    }

    exit(0);
}

int ssu_daemon_init(void){
    pid_t pid;
    int fd,maxfd;

    if((pid=fork())<0){
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if(pid!=0)
        exit(0);

    pid = getpid();
    printf("process %d running as daemon\n",pid);//대몬 프로세스의 pid 출력
    setsid();//새로운 세션을 생성
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);//시그널 연결을 끊음
    maxfd=getdtablesize();//테이블 사이즈를 가져옴

    for(fd=0;fd<maxfd;fd++)
        close(fd);//열려있는 fd를 전부 닫음

    umask(0);//umask를 0으로 설정
    chdir("/");//최상위 디렉토리로 작업경로 이동
    fd = open("/dev/null",O_RDWR);//표준 입출력과 에러를 다시설
    dup(0);
    dup(0);
    return 0;
}
